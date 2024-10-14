#include "pgpch.h"
#include "pg_bina_writer.h"

namespace Pagoda::Database {
    void BinaWriter::AddStruct(void* structData, size_t size) {
        // Store the size of the struct.
        this->m_structSizeMap.insert({structData, size});

        this->m_structs.push_back(structData);
        this->m_structSize += size;
    }

    void BinaWriter::AddString(char* str) {
        // Get string size, accounting for null termination.
        size_t size = strlen(str) + 1;

        this->m_structSizeMap.insert({str, size});

        this->m_strings.push_back(str);
        this->m_stringTableSize += size;
    }

    void BinaWriter::AddStringVector(std::vector<char*>& structData) {
        this->AddStruct(&structData[0], sizeof(char*) * (unsigned int)structData.size());

        for (char* s : structData) {
            this->AddString(s);
        }
    }
    
    void BinaWriter::WriteData(char** offset, std::vector<void*> structs) {
        for (void* structData : structs) {
            // Write the struct to the block.
            unsigned long long structSize = this->m_structSizeMap.at(structData);
            memcpy(*offset, structData, structSize);

            // Cache the current block offset, so that pointers to this struct can be fixed later.
            this->m_offsetMap.insert({structData, *offset});

            // Set the block pointer to the end of the written struct data.
            *offset += structSize;
        }

        // Align the written struct data to 4 bytes.
        *offset += GetAlignment((size_t)*offset, 4);
    }

    void BinaWriter::Write(const char filePath[]) {
        unsigned int stringAlignment = GetAlignment(this->m_stringTableSize, 4);

        // Allocate enough bytes for bina header and node data.
        size_t heapSize = sizeof(BINAHeader) + sizeof(NodeHeader) + this->m_structSize + this->m_stringTableSize + stringAlignment;
        char* pBinaNode = new char[heapSize];

        // Initialise the allocated heap space to zero.
        memset(pBinaNode, 0, heapSize);

        std::ofstream outFile;
        outFile.open(filePath, std::ios::out | std::ios::binary);

        BINAHeader* binaHeader = (BINAHeader*)pBinaNode;
        NodeHeader* nodeHeader = (NodeHeader*)(pBinaNode + sizeof(BINAHeader));

        // Set header information.
        binaHeader->header = binaSig;
        memcpy(binaHeader->version, binaVer, 3);
        nodeHeader->signature = dataSig;
        binaHeader->endianFlag = 'L';

        binaHeader->nodeCount = 1;

        char* pCurrentOffset = (char*)nodeHeader + sizeof(NodeHeader);

        char* pNodeBody = pCurrentOffset;

        // Write data and string table.
        WriteData(&pCurrentOffset, this->m_structs);
        WriteData(&pCurrentOffset, this->m_strings);

        // Set string table information.
        nodeHeader->stringTableOffset = (unsigned int)this->m_structSize;
        nodeHeader->stringTableLength = (unsigned int)this->m_stringTableSize + stringAlignment;

        nodeHeader->additionalDataLength = ADD_DATA_LENGTH;

        // Map pointers to the correct structs in the file.
        FixPointers(pNodeBody);

        std::stringstream offsetTableStream = DatabaseUtils::GenerateBINAOffsetTable(this->m_offsets);
        DatabaseUtils::Align(offsetTableStream);

        // Set BINA and node size, then write the file.
        nodeHeader->offsetTableLength = (unsigned int)offsetTableStream.str().size();
        nodeHeader->length = sizeof(NodeHeader) + (unsigned int)this->m_structSize + (unsigned int)this->m_stringTableSize + stringAlignment + nodeHeader->offsetTableLength;
        binaHeader->fileSize = sizeof(BINAHeader) + (size_t)nodeHeader->length;
        outFile.write(pBinaNode, binaHeader->fileSize - nodeHeader->offsetTableLength);

        outFile << offsetTableStream.str();

        outFile.close();

        delete[] pBinaNode;
    }

    unsigned int BinaWriter::GetAlignment(size_t count, unsigned int factor) {
        size_t val = factor - (count % factor);
        return val == factor ? 0 : val;
    }

    void BinaWriter::FixPointers(char* nodeBody) {
        unsigned int count = (unsigned int)(this->m_structSize / sizeof(uint32_t));
        char** ptr = (char**)nodeBody;

        unsigned long long lastOffset = 0;
        for (unsigned int i = 0; i < count; i++) {
            char** ppCurrentPos = ptr + i;
            auto blockOffset = this->m_offsetMap.find((void*)*ppCurrentPos);
            if (blockOffset != m_offsetMap.end()) {
                const size_t offsetFromBlockStart = blockOffset->second - nodeBody;
                *(ppCurrentPos) = (char*)offsetFromBlockStart;

                const size_t offset = ((size_t)ppCurrentPos - (size_t)nodeBody);
                this->m_offsets.push_back(offset - lastOffset);

                lastOffset = offset;
            }
        }
    }
}