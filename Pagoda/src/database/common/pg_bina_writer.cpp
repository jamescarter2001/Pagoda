#include "pgpch.h"
#include "pg_bina_writer.h"

#include "base/collection/pg_collections.h"

namespace Pagoda::Database {
    BinaWriter::BinaWriter() {

    }

    BinaWriter::~BinaWriter() {
        for (const auto& n : m_nodes) {
            delete n;
        }
    }

    void BinaWriter::BeginNode() {
        m_activeNode = new NodeInfo();
    }

    void BinaWriter::EndNode() {
        // Calculate node data.
        m_activeNode->stringTableSize += GetAlignment(m_activeNode->stringTableSize, 0x4);
        std::vector<size_t> offsets = SeekOffsets(m_activeNode);
        m_activeNode->offsetTable = DatabaseUtils::GenerateBINAOffsetTable(offsets);
        m_activeNode->offsetTableSize = m_activeNode->offsetTable.str().size();

        size_t nodeLength = sizeof(NodeHeader) + m_activeNode->structsSize + m_activeNode->stringTableSize + m_activeNode->offsetTableSize;
        m_activeNode->padding = GetAlignment(nodeLength, 0x10);
        m_activeNode->length = nodeLength + m_activeNode->padding;

        m_nodes.emplace_back(m_activeNode);
        m_activeNode = nullptr;
    }

    void BinaWriter::AddStruct(const void* const structData, const size_t size) {
        // Store the size of the struct.
        const size_t alignedSize = size + GetAlignment(size, sizeof(size_t));
        m_activeNode->structs.Put(structData, StructHandle{structData, true, m_activeNode->structsSize, size, alignedSize, StructHandleType::STRUCT_HANDLE_TYPE_STD});

        m_activeNode->structsSize += alignedSize;
    }

    void BinaWriter::AddString(const char* const str) {
        // Get string size, accounting for null termination.
        const size_t size = strlen(str) + 1;

        // Have we already seen this string?
        const void* const* existing = Base::Collections::FindFirst<const void*>(m_activeNode->structs.Keys(), [str](const void* const& p) { return strcmp((char*)p, str) == 0; });
        if (existing != nullptr) {
            const StructHandle& existingHandle = m_activeNode->structs.Get(*existing);
            m_activeNode->structs.Put(str, StructHandle{existingHandle.pStruct, false, existingHandle.pOffset, existingHandle.size, existingHandle.alignedSize, existingHandle.type});
            return;
        }
        
        /**
        * Store the struct.
        * Since we won't know where the string table will be until all other structs have been written,
        * so we store the current string table length instead (m_stringTableSize).
        */
        m_activeNode->structs.Put(str, StructHandle{str, true, m_activeNode->stringTableSize, size, size, StructHandleType::STRUCT_HANDLE_TYPE_STR});

        m_activeNode->stringTableSize += size;
    }

    void BinaWriter::AddStringVector(const std::vector<char*>& structData) {
        this->AddStruct(&structData[0], sizeof(char*) * structData.size());

        for (const auto& s : structData) {
            this->AddString(s);
        }
    }

    void BinaWriter::WriteData(char** const offset, const Base::OrderedMap<const void*, StructHandle>& structs, const StructHandleType& handleType, const size_t size) {
        const char* startOffset = *offset;
        WriteData(offset, structs, handleType);
        const size_t advancedBy = reinterpret_cast<size_t>(*offset) - (size_t)startOffset;
        *offset += (size - advancedBy);
    }
    
    void BinaWriter::WriteData(char** const offset, const Base::OrderedMap<const void*, StructHandle>& structs, const StructHandleType& handleType) {
        for (auto& k : structs.Keys()) {
            const StructHandle& sh = structs.Get(k);

            if (sh.type != handleType || !sh.writable) {
                // no-op
                continue;
            }

            // Write the struct to the block.
            memcpy(*offset, sh.pStruct, sh.size);

            // Set the block pointer to the end of the written struct data.
            *offset += sh.alignedSize;
        }
    }

    void BinaWriter::Write(const char filePath[]) {
        size_t fileSize = sizeof(BINAHeader);

        for (auto& handle : m_nodes) {
            fileSize += handle->length;
        }

        // Allocate heap buffer.
        char* pBinaNode = new char[fileSize];

        // Initialise the allocated heap space to zero.
        memset(pBinaNode, 0, fileSize);

        std::ofstream outFile;
        outFile.open(filePath, std::ios::out | std::ios::binary);

        BINAHeader* binaHeader = (BINAHeader*)pBinaNode;

        // Set BINA header information.
        binaHeader->header = binaSig;
        memcpy(binaHeader->version, binaVer, 3);
        binaHeader->endianFlag = 'L';
        binaHeader->fileSize = (uint32_t)fileSize;
        binaHeader->nodeCount = (short)m_nodes.size();

        char* pCurrentOffset = pBinaNode + sizeof(BINAHeader);

        for (auto& handle : m_nodes) {
            NodeHeader* nodeHeader = reinterpret_cast<NodeHeader*>(pCurrentOffset);

            // Set node header information.
            nodeHeader->signature = dataSig;
            nodeHeader->length = (unsigned int)handle->length;
            nodeHeader->additionalDataLength = ADD_DATA_LENGTH;
            nodeHeader->stringTableOffset = (unsigned int)handle->structsSize;
            nodeHeader->stringTableLength = (unsigned int)handle->stringTableSize;
            nodeHeader->offsetTableLength = (unsigned int)handle->offsetTableSize;

            pCurrentOffset += sizeof(NodeHeader);
            char* const pNodeBody = pCurrentOffset;

            // Write data, string table and offset table.
            WriteData(&pCurrentOffset, handle->structs, StructHandleType::STRUCT_HANDLE_TYPE_STD);
            WriteData(&pCurrentOffset, handle->structs, StructHandleType::STRUCT_HANDLE_TYPE_STR, handle->stringTableSize);
            memcpy(pCurrentOffset, handle->offsetTable.str().c_str(), handle->offsetTableSize);
            
            // Substitute struct pointers for relative file offsets.
            FixPointers(pNodeBody, handle);
            
            pCurrentOffset += handle->offsetTableSize + handle->padding;
        }

        outFile.write(pBinaNode, fileSize);
        outFile.close();

        delete[] pBinaNode;
    }

    size_t BinaWriter::GetAlignment(const size_t count, const unsigned int factor) {
        size_t val = factor - (count % factor);
        return val == factor ? 0 : val;
    }

    std::vector<size_t> BinaWriter::SeekOffsets(const NodeInfo* const nodeInfo) {
        std::vector<size_t> result;

        size_t lastOffset = 0;
        size_t currentSize = 0;
        for (auto& k : nodeInfo->structs.Keys()) {
            const StructHandle& h = nodeInfo->structs.Get(k);

            if (h.type != STRUCT_HANDLE_TYPE_STD) {
                // no-op
                continue;
            }

            for (size_t i = 0; i < h.size; i += sizeof(size_t)) {
                const void *const *const pStruct = reinterpret_cast<const void *const *const>(reinterpret_cast<const char* const>(h.pStruct) + i);
                if (nodeInfo->structs.Contains(*pStruct)) {
                    const size_t offset = currentSize + i;
                    result.push_back(offset - lastOffset);
                    lastOffset = offset;
                }
            }

            currentSize += h.alignedSize;
        }

        return result;
    }

    void BinaWriter::FixPointers(char* const pNodeBody, NodeInfo* const nodeInfo) {
        char* pCurrentPos = pNodeBody;

        std::vector<size_t> offsets = SeekOffsets(nodeInfo);
        for (unsigned int i = 0; i < offsets.size(); i++) {
            pCurrentPos += offsets.at(i);
            char** const ppPosition = reinterpret_cast<char**>(pCurrentPos);

            const StructHandle& h = nodeInfo->structs.Get(*ppPosition);

            // Replace the pointer with the correct file offset.
            switch (h.type) {
                case StructHandleType::STRUCT_HANDLE_TYPE_STD:
                    *(ppPosition) = (char*)h.pOffset;
                    break;
                case StructHandleType::STRUCT_HANDLE_TYPE_STR:
                    *(ppPosition) = (char*)(nodeInfo->structsSize + h.pOffset);
                    break;
            }
        }
    }
}