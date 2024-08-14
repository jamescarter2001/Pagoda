#include "pgpch.h"
#include "pg_bina_template_converter.h"

namespace Pagoda::Database {
    BINATemplateConverter::BINATemplateConverter(unsigned int ptrSize) : m_ptrSize(ptrSize) {
        this->m_symbolSizeMap = {
            { BINA_SYM_U8,  sizeof(uint8_t)  },
            { BINA_SYM_U16, sizeof(uint16_t) },
            { BINA_SYM_U32, sizeof(uint32_t) },
            { BINA_SYM_U64, sizeof(uint64_t) },
            { BINA_SYM_S8,  sizeof(int8_t)   },
            { BINA_SYM_S16, sizeof(int16_t)  },
            { BINA_SYM_S32, sizeof(int32_t)  },
            { BINA_SYM_S64, sizeof(int64_t)  },
            { BINA_SYM_F32, sizeof(float)    },
            { BINA_SYM_F64, sizeof(double)   },
            { BINA_SYM_STR, ptrSize          },
            { BINA_SYM_REF, ptrSize          }
        };
    }
    BINATemplateConverter::~BINATemplateConverter() {}

    size_t BINATemplateConverter::GetSizeOfType(std::string type) {
        return m_symbolSizeMap.at(type);
    }

    void BINATemplateConverter::Write(char** dst, void* data, std::string type) {
        size_t typeSize = GetSizeOfType(type);
        memcpy(*dst, data, typeSize);
        *dst += typeSize;
    }

    void BINATemplateConverter::WriteData(char** ppCurrentOffset, std::string type, std::string data, bool bigEndian) {
        if (type == BINA_SYM_U8) {
            uint8_t d = (uint8_t)std::stoi(data);
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_U16) {
            uint16_t d = (uint16_t)std::stoi(data);
            if (bigEndian) { d = _byteswap_ushort(d); }
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_U32) {
            uint32_t d = (uint32_t)std::stoi(data);
            if (bigEndian) { d = _byteswap_ulong(d); }
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_U64) {
            uint64_t d = (uint64_t)std::stoull(data);
            if (bigEndian) { d = _byteswap_uint64(d); }
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_S8) {
            int8_t d = (int8_t)std::stoi(data);
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_S16) {
            int16_t d = (int16_t)std::stoi(data);
            if (bigEndian) { d = _byteswap_ushort(d); }
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_S32) {
            int32_t d = (int32_t)std::stoi(data);
            if (bigEndian) { d = _byteswap_ulong(d); }
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_S64) {
            int64_t d = (int64_t)std::stoll(data);
            if (bigEndian) { d = _byteswap_uint64(d); }
            this->Write(ppCurrentOffset, &d, type);
        }

        if (type == BINA_SYM_F32) {
            float fl = (float)std::stof(data);
            unsigned long* d = (unsigned long*)&fl;
            if (bigEndian) { *d = _byteswap_ulong(*d); }
            this->Write(ppCurrentOffset, d, type);
        }

        if (type == BINA_SYM_F64) {
            double doub = (double)std::stod(data);
            unsigned long long* d = (unsigned long long*)&doub;
            if (bigEndian) { *d = _byteswap_uint64(*d); }
            this->Write(ppCurrentOffset, d, type);
        }

        if (type == BINA_SYM_STR) {
            auto str = this->m_stringTablePositionMap.at(data);

            if (this->m_ptrSize == PTR_SIZE_32) {
                uint32_t offset = (uint32_t)(this->m_dataBlockSize + str);
                if (bigEndian) { offset = _byteswap_ulong(offset); }
                this->Write(ppCurrentOffset, &offset, BINA_SYM_U32);
            }

            if (this->m_ptrSize == PTR_SIZE_64) {
                uint64_t offset = this->m_dataBlockSize + str;
                if (bigEndian) { offset = _byteswap_uint64(offset); }
                this->Write(ppCurrentOffset, &offset, BINA_SYM_U64);
            }
        }

        if (type == BINA_SYM_REF) {
            if (this->m_ptrSize == 4) {
                uint32_t offset = this->m_offsetMap.at(data);
                if (bigEndian) { offset = _byteswap_ulong(offset); }
                this->Write(ppCurrentOffset, &offset, BINA_SYM_U32);
            }

            if (this->m_ptrSize == 8) {
                uint64_t offset = this->m_offsetMap.at(data);
                if (bigEndian) { offset = _byteswap_uint64(offset); }
                this->Write(ppCurrentOffset, &offset, BINA_SYM_U64);
            }
        }
    }

    void BINATemplateConverter::ConvertTemplateAndSave(const char src[], const char dest[], bool bigEndian) {
        this->InspectTemplate(src);

        std::ifstream srcFile(src, std::ios::in);
        unsigned int count = 0;

        size_t fileLength = sizeof(BINAHeader) +
                            sizeof(NodeHeader) +
                            this->m_dataBlockSize +
                            this->m_stringTable.str().size() +
                            this->m_offsetTable.str().size();

        char* outHeap = new char[fileLength];
        memset(outHeap, 0, fileLength);

        BINAHeader* bh = (BINAHeader*)outHeap;
        NodeHeader* nh = (NodeHeader*)(outHeap + sizeof(BINAHeader));

        bh->header = binaSig;
        memcpy(bh->version, binaVer, 3);
        bh->endianFlag = 'L';
        bh->fileSize = (unsigned int)fileLength;
        bh->nodeCount = 1;

        nh->signature = dataSig;
        nh->length = (unsigned int)(fileLength - sizeof(BINAHeader));
        nh->stringTableOffset = (unsigned int)this->m_dataBlockSize;
        nh->stringTableLength = (unsigned int)(this->m_stringTable.str().size());
        nh->offsetTableLength = (unsigned int)(this->m_stringTable.str().size());
        nh->additionalDataLength = ADD_DATA_LENGTH;

        char* pCurrentOffset = (char*)nh + sizeof(NodeHeader);

        try {
            std::string line;
            while (std::getline(srcFile, line)) {
                if (line.at(0) == '\t') {
                    std::string entry = line.substr(1, line.size() - 1);

                    std::vector<std::string> fragments = Base::Strings::Split(entry, " ");
                    if (fragments.size() < 2) {
                        continue;
                    }
                    for (int i = 1; i < fragments.size(); i++) {
                        this->WriteData(&pCurrentOffset, fragments[0], fragments[i], bigEndian);
                    }
                }
            }

            memcpy(pCurrentOffset, this->m_stringTable.str().c_str(), this->m_stringTable.str().size());
            pCurrentOffset += this->m_stringTable.str().size();
            memcpy(pCurrentOffset, this->m_offsetTable.str().c_str(), this->m_offsetTable.str().size());

            unsigned int fileSize = bh->fileSize;

            if (bigEndian) {
                SwapBINAHeader(bh);
                SwapNodeHeader(nh);
            }

            std::ofstream outFile(dest, std::ios::out | std::ios::binary);
            outFile.write(outHeap, fileSize);
            outFile.close();

        } catch (...) {
            std::cout << "Invalid template file." << std::endl;
        }
        delete[] outHeap;
    }

    void BINATemplateConverter::InspectTemplate(const char src[]) {
        std::ifstream srcFile(src, std::ios::in);

        std::string line;
        unsigned long long lastOffset = 0;
        while (std::getline(srcFile, line)) {
            if (line.at(0) == '\t') {
                std::string entry = line.substr(1, line.size() - 1);

                std::vector<std::string> fragments = Base::Strings::Split(entry, " ");

                // Ignore blank lines.
                if (fragments.size() < 2) { continue; }

                for (int i = 1; i < fragments.size(); i++) {
                    // If string type, append to string table.
                    if (fragments[0] == BINA_SYM_STR) {
                        auto stringEntry = this->m_stringTablePositionMap.find(fragments[i]);

                        // Is this the first time seeing this string?
                        if (stringEntry == this->m_stringTablePositionMap.end()) {
                            this->m_stringTablePositionMap.insert({fragments[i], (unsigned int)this->m_stringTable.str().size()});

                            this->m_stringTable << fragments[i] << '\0';
                        }
                    }

                    // If pointer type, append to offsets.
                    if (fragments[0] == BINA_SYM_STR || fragments[0] == BINA_SYM_REF) {
                        this->m_offsets.push_back(this->m_dataBlockSize - lastOffset);
                        lastOffset = this->m_dataBlockSize;
                    }

                    // Append to data block size.
                    this->m_dataBlockSize += GetSizeOfType(fragments[0]);
                }
            }

            std::vector<std::string> fragments = Base::Strings::Split(line, ":");
            if (fragments.size() == 2) {
                this->m_offsetMap.insert({fragments[0], (unsigned int)this->m_dataBlockSize});
            }
        }
        srcFile.close();

        this->m_offsetTable = DatabaseUtils::GenerateBINAOffsetTable(this->m_offsets);

        // Align tables to 4 bytes.
        DatabaseUtils::Align(this->m_stringTable);
        DatabaseUtils::Align(this->m_offsetTable);
    }
}