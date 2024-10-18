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

    void BinaWriter::BeginNode(const OffsetType& offsetType) {
        m_activeNode = new NodeInfo();
        m_activeNode->offsetType = offsetType;
    }

    void BinaWriter::BeginNode() {
        BeginNode(OFFSET_TYPE_RELATIVE);
    }

    void BinaWriter::EndNode() {
        // If misc data has been declared, we align the string table to 16 bytes. Otherwise, 4 bytes is sufficient.
        m_activeNode->stringTableSize += GetAlignment(m_activeNode->stringTableSize, m_activeNode->miscSize > 0 ? 0x10: 0x4);
        std::vector<size_t> offsets = SeekOffsets(m_activeNode);

        // Convert the first offset to an absolute offset if required.
        if (m_activeNode->offsetType == OFFSET_TYPE_ABSOLUTE) {  
            const size_t sizeOfPreviousNodes = Base::Collections::Sum<NodeInfo*>(m_nodes, [](const NodeInfo* const& ni) { return ni->length; });
            offsets.at(0) = offsets.at(0) + sizeOfPreviousNodes + GetFileHeaderSize() + GetNodeHeaderSize();
        };
        m_activeNode->offsetTable = DatabaseUtils::GenerateBINAOffsetTable(offsets);
        m_activeNode->offsetTableSize = m_activeNode->offsetTable.str().size();

        size_t nodeLength = GetNodeHeaderSize() + m_activeNode->structsSize + m_activeNode->stringTableSize + m_activeNode->miscSize + m_activeNode->offsetTableSize;
        const size_t offsetTablePadding = GetAlignment(nodeLength, 0x10);
        m_activeNode->offsetTableSize += offsetTablePadding;
        m_activeNode->length = nodeLength + offsetTablePadding;

        m_nodes.emplace_back(m_activeNode);
        m_activeNode = nullptr;
    }

    void BinaWriter::AddStruct(const void* const structData, const size_t size) {
        // Store the size of the struct.
        const size_t alignedSize = size + GetAlignment(size, 0x10);
        m_activeNode->structs.Put(structData, StructHandle{structData, true, m_activeNode->structsSize, size, alignedSize, StructHandleType::STRUCT_HANDLE_TYPE_STD});

        m_activeNode->structsSize += alignedSize;
    }

    void BinaWriter::AddMisc(const void* const structData, const size_t size) {
        // Store the size of the blob.
        const size_t alignedSize = size + GetAlignment(size, 0x10);
        m_activeNode->structs.Put(structData, StructHandle{structData, true, m_activeNode->miscSize, size, alignedSize, StructHandleType::STRUCT_HANDLE_TYPE_MSC});

        m_activeNode->miscSize += alignedSize;
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

    void BinaWriter::WriteFileHeader(char** const offset, const size_t fileSize, const size_t nodeCount) {
        BINAHeader* binaHeader = reinterpret_cast<BINAHeader*>(*offset);

        // Set BINA header information.
        binaHeader->header = binaSig;
        memcpy(binaHeader->version, binaVer, 3);
        binaHeader->endianFlag = 'L';
        binaHeader->fileSize = (uint32_t)fileSize;
        binaHeader->nodeCount = (short)m_nodes.size();

        *offset += sizeof(BINAHeader);
    }

    void BinaWriter::WriteNodeHeader(char** const offset, const NodeInfo* const handle) {
        NodeHeader* nodeHeader = reinterpret_cast<NodeHeader*>(*offset);

        // Set node header information.
        nodeHeader->signature = dataSig;
        nodeHeader->length = (unsigned int)handle->length;
        nodeHeader->additionalDataLength = ADD_DATA_LENGTH;
        nodeHeader->stringTableOffset = (unsigned int)handle->structsSize;
        nodeHeader->stringTableLength = (unsigned int)handle->stringTableSize;
        nodeHeader->offsetTableLength = (unsigned int)handle->offsetTableSize;

        *offset += sizeof(NodeHeader);
    }

    void BinaWriter::Write(const char filePath[]) {
        size_t fileSize = sizeof(BINAHeader);

        for (auto& handle : m_nodes) {
            fileSize += handle->length;
        }

        // Allocate heap buffer.
        char* pBinaNode = new char[fileSize];

        // Initialize the allocated heap space to zero.
        memset(pBinaNode, 0, fileSize);

        std::ofstream outFile;
        outFile.open(filePath, std::ios::out | std::ios::binary);

        char* pCurrentOffset = pBinaNode;
        WriteFileHeader(&pCurrentOffset, fileSize, m_nodes.size());

        for (auto& handle : m_nodes) {
            WriteNodeHeader(&pCurrentOffset, handle);

            char* const pNodeBody = pCurrentOffset;

            // Write data, string table, blobs and offset table.
            WriteData(&pCurrentOffset, handle->structs, StructHandleType::STRUCT_HANDLE_TYPE_STD);
            WriteData(&pCurrentOffset, handle->structs, StructHandleType::STRUCT_HANDLE_TYPE_STR, handle->stringTableSize);
            WriteData(&pCurrentOffset, handle->structs, StructHandleType::STRUCT_HANDLE_TYPE_MSC, handle->miscSize);
            memcpy(pCurrentOffset, handle->offsetTable.str().c_str(), handle->offsetTableSize);
            
            // Substitute struct pointers for relative file offsets.
            const size_t fromOffset = handle->offsetType == OFFSET_TYPE_ABSOLUTE ? (reinterpret_cast<size_t>(pNodeBody) - reinterpret_cast<size_t>(pBinaNode)) : 0;
            FixPointers(pNodeBody, handle, fromOffset);
            
            pCurrentOffset += handle->offsetTableSize;
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
                const void *const *const pStruct = reinterpret_cast<const void* const *const>(reinterpret_cast<const char* const>(h.pStruct) + i);
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

    void BinaWriter::FixPointers(char* const pNodeBody, const NodeInfo *const nodeInfo, const size_t fromOffset) {
        char* pCurrentPos = pNodeBody;

        std::vector<size_t> offsets = SeekOffsets(nodeInfo);
        for (unsigned int i = 0; i < offsets.size(); i++) {
            pCurrentPos += offsets.at(i);
            char** const ppPosition = reinterpret_cast<char**>(pCurrentPos);

            const StructHandle& h = nodeInfo->structs.Get(*ppPosition);

            // Replace the pointer with the correct file offset.
            switch (h.type) {
                case StructHandleType::STRUCT_HANDLE_TYPE_STD:
                    *(ppPosition) = (char*)(fromOffset + h.pOffset);
                    break;
                case StructHandleType::STRUCT_HANDLE_TYPE_STR:
                    *(ppPosition) = (char*)(fromOffset + nodeInfo->structsSize + h.pOffset);
                    break;
                case StructHandleType::STRUCT_HANDLE_TYPE_MSC:
                    *(ppPosition) = (char*)(fromOffset + nodeInfo->structsSize + nodeInfo->stringTableSize + h.pOffset);
                    break;
            }
        }
    }
}