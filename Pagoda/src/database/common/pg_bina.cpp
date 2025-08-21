#include "pgpch.h"

#include "pg_bina.h"

namespace Pagoda::Database {
    std::vector<size_t*> Node::SeekOffsets(data_t* const start, char* offsetTable, unsigned int offsetTableLength) {
        data_t* current = start;

        std::vector<size_t*> offsets;
        unsigned int i = 0;
        while (i < offsetTableLength) {
            if (*offsetTable & BINA_OFFSET_6) {
                unsigned long o = 0;

                uint8_t* dat = (uint8_t*)offsetTable;
                o = *dat - 0x40;
                o = o << 2;

                current = current + o;

                offsets.push_back(reinterpret_cast<size_t*>(current));
                offsetTable++;
                i++;
            } else if (*offsetTable & BINA_OFFSET_14) {
                unsigned long o = 0;

                uint16_t* dat = (uint16_t*)offsetTable;
                o = _byteswap_ushort(*dat);
                o = o - 0x8000;
                o = o << 2;

                current = current + o;

                offsets.push_back(reinterpret_cast<size_t*>(current));
                offsetTable = offsetTable + 2;
                i = i + 2;
            } else if (*offsetTable & BINA_OFFSET_30) {
                unsigned long o = 0;

                uint32_t* dat = (uint32_t*)offsetTable;
                o = _byteswap_ulong(*dat);
                o = o - 0xC0000000;
                o = o << 2;

                current = current + o;

                offsets.push_back(reinterpret_cast<size_t*>(current));
                offsetTable = offsetTable + 4;
                i = i + 4;
            } else if (*offsetTable == '\0') {
                // End of BINA200 offset table.
                return offsets;
            } else {
                std::cout << "Invalid offset table!" << std::endl;
                return {};
            }
        }
        return offsets;
    }

    std::vector<size_t*> Node::PrintOffsets(data_t* const start, char* offsetTable, unsigned int offsetTableLength) {
        std::vector<size_t*> offsets = Node::SeekOffsets(start, offsetTable, offsetTableLength);

        for (const auto& o : offsets) {
            std::cout << std::hex << o << std::endl;
        }

        return offsets;
    }

    std::vector<size_t*> Node::SeekOffsets(node_t* node) {
        NodeHeader nh = *(NodeHeader*)node;

        char* offsetTable = node + sizeof(nh) + nh.stringTableOffset + nh.stringTableLength;
        data_t* start = node + sizeof(nh);

        return Node::SeekOffsets(start, offsetTable, nh.offsetTableLength);
    }

}