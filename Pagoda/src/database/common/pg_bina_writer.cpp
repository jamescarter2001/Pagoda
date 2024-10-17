#include "pgpch.h"
#include "pg_bina_writer.h"

#include "base/collection/pg_collections.h"

namespace Pagoda::Database {
    void BinaWriter::AddStruct(void* structData, size_t size) {
        // Store the size of the struct.
        const size_t alignedSize = size + GetAlignment(size, 4);
        m_structs.Put(structData, StructHandle{structData, true, m_structSize, size, alignedSize, StructHandleType::STRUCT_HANDLE_TYPE_STD});

        this->m_structSize += alignedSize;
    }

    void BinaWriter::AddString(char* str) {
        // Get string size, accounting for null termination.
        const size_t size = strlen(str) + 1;

        // Have we already seen this string?
        void* const* existing = Base::Collections::FindFirst<void*>(m_structs.Keys(), [str](void* const& p) { return strcmp((char*)p, str) == 0; });

        if (existing != nullptr) {
            const StructHandle& existingHandle = m_structs.Get(*existing);
            m_structs.Put(str, StructHandle{existingHandle.pStruct, false, existingHandle.pOffset, existingHandle.size, existingHandle.alignedSize, existingHandle.type});
            return;
        }
        
        /**
        * Store the struct.
        * Since we won't know where the string table will be until all other structs have been written,
        * so we store the current string table length instead (m_stringTableSize).
        */
        m_structs.Put(str, StructHandle{str, true, m_stringTableSize, size, size, StructHandleType::STRUCT_HANDLE_TYPE_STR});

        this->m_stringTableSize += size;
    }

    void BinaWriter::AddStringVector(std::vector<char*>& structData) {
        this->AddStruct(&structData[0], sizeof(char*) * (unsigned int)structData.size());

        for (char* s : structData) {
            this->AddString(s);
        }
    }
    
    void BinaWriter::WriteData(char** offset, const Base::OrderedMap<void*, StructHandle>& structs, const StructHandleType& handleType) {
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
        unsigned int alignedStringTableSize = m_stringTableSize + GetAlignment(m_stringTableSize, 4);

        // Allocate enough bytes for bina header and node data.
        size_t heapSize = sizeof(BINAHeader) + sizeof(NodeHeader) + this->m_structSize + alignedStringTableSize;
        char* pBinaNode = new char[heapSize];

        // Initialise the allocated heap space to zero.
        memset(pBinaNode, 0, heapSize);

        std::ofstream outFile;
        outFile.open(filePath, std::ios::out | std::ios::binary);

        BINAHeader* binaHeader = (BINAHeader*)pBinaNode;
        NodeHeader* nodeHeader = (NodeHeader*)(pBinaNode + sizeof(BINAHeader));

        // Set BINA header information.
        binaHeader->header = binaSig;
        memcpy(binaHeader->version, binaVer, 3);
        binaHeader->endianFlag = 'L';
        binaHeader->nodeCount = 1;

        // Set node header information.
        nodeHeader->signature = dataSig;
        nodeHeader->additionalDataLength = ADD_DATA_LENGTH;
        nodeHeader->stringTableOffset = (unsigned int)this->m_structSize;
        nodeHeader->stringTableLength = (unsigned int)alignedStringTableSize;

        char* pCurrentOffset = (char*)nodeHeader + sizeof(NodeHeader);
        char* pNodeBody = pCurrentOffset;

        // Write data and string table.
        WriteData(&pCurrentOffset, this->m_structs, StructHandleType::STRUCT_HANDLE_TYPE_STD);
        WriteData(&pCurrentOffset, this->m_structs, StructHandleType::STRUCT_HANDLE_TYPE_STR);

        // Map pointers to the correct structs in the file.
        FixPointers(pNodeBody);

        // Calculate offset table.
        std::stringstream offsetTableStream = DatabaseUtils::GenerateBINAOffsetTable(this->m_offsets);
        const size_t offsetTableLength = offsetTableStream.str().size();
        nodeHeader->offsetTableLength = (unsigned int)offsetTableLength;

        // Calculate node length.
        size_t nodeLength = sizeof(NodeHeader) + this->m_structSize + alignedStringTableSize + offsetTableLength;

        // Calculate required offset table padding;
        const size_t offsetTablePadding = GetAlignment(nodeLength, 16);
        nodeLength += offsetTablePadding;

        // Apply required offset table padding.
        for (size_t i = 0; i < offsetTablePadding; i++) {
            offsetTableStream << '\0';
        }

        // Set BINA and node size, then write the file.
        nodeHeader->length = nodeLength;
        binaHeader->fileSize = sizeof(BINAHeader) + nodeLength;
        outFile.write(pBinaNode, binaHeader->fileSize - offsetTableLength - offsetTablePadding);

        outFile << offsetTableStream.str();

        outFile.close();

        delete[] pBinaNode;
    }

    size_t BinaWriter::GetAlignment(size_t count, unsigned int factor) {
        size_t val = factor - (count % factor);
        return val == factor ? 0 : val;
    }

    void BinaWriter::FixPointers(char* nodeBody) {
        unsigned int count = (unsigned int)(this->m_structSize / sizeof(size_t));
        char** ptr = (char**)nodeBody;

        unsigned long long lastOffset = 0;
        for (unsigned int i = 0; i < count; i++) {
            char** ppCurrentPos = ptr + i;

            if (m_structs.Contains((void*)*ppCurrentPos)) {
                const StructHandle& h = m_structs.Get((void*)*ppCurrentPos);

                // Replace the pointer with the correct file offset.
                switch (h.type) {
                    case StructHandleType::STRUCT_HANDLE_TYPE_STD:
                        *(ppCurrentPos) = (char*)h.pOffset;
                        break;
                    case StructHandleType::STRUCT_HANDLE_TYPE_STR:
                        *(ppCurrentPos) = (char*)(m_structSize + h.pOffset);
                }
                
                // Cache the position of the written offset, in preparation for writing the offset table.
                const size_t offset = ((size_t)ppCurrentPos - (size_t)nodeBody);
                this->m_offsets.push_back(offset - lastOffset);

                lastOffset = offset;
            }
        }
    }
}