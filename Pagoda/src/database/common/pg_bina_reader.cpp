#include "pgpch.h"

#include "pg_bina_reader.h"
#include "pg_pacx.h"

#include "lz4.h"

namespace Pagoda::Database {
    BinaReader::BinaReader() {
    }

    BinaReader::~BinaReader() {
    }

    std::vector<data_t*> BinaReader::Read(std::string path) {
        std::ifstream file(path, std::ios::binary | std::ios::in);

        bina_t* bina;
        std::vector<data_t*> dataBlocks;

        PACV3Header pacv3Header;
        READ_STRUCT(file, pacv3Header);
        file.seekg(0, std::ios::beg);

        if (pacv3Header.signature == pacSig && VersionEquals(pacv3Header.version, "301")) {
            std::cout << "Reading PAC: V3" << std::endl;
            bina = new bina_t[pacv3Header.fileSize];
            file.read(bina, pacv3Header.fileSize);

            return ReadPACV3(bina);
        }

        PACV403Header pacv4Header;
        READ_STRUCT(file, pacv4Header);
        file.seekg(0, std::ios::beg);

        if (pacv4Header.signature == pacSig && VersionEquals(pacv4Header.version, "403")) {
            std::cout << "Reading PAC: V4" << std::endl;
            bina = new bina_t[pacv4Header.fileSize];
            file.read(bina, pacv4Header.fileSize);

            return ReadPACV403(bina);
        }

        BINAV1Header binav1Header;
        READ_STRUCT(file, binav1Header);
        file.seekg(0, std::ios::beg);

        if (binav1Header.signature == binaSig) {
            std::cout << "Reading BINA: V1" << std::endl;
            bina = new bina_t[binav1Header.fileSize];
            file.read(bina, binav1Header.fileSize);

            return ReadV1(bina);
        }

        BINAHeader binaHeader;
        READ_STRUCT(file, binaHeader);
        file.seekg(0, std::ios::beg);

        if (binaHeader.header == binaSig) {
            std::cout << "Reading BINA: V2" << std::endl;
            bina = new bina_t[binaHeader.fileSize];
            file.read(bina, binaHeader.fileSize);

            return ReadV2(bina);
        }
        
        return {};
    }

    void BinaReader::FixPointers(data_t* dataBlock, const std::vector<size_t*>& offsets) {
        for (auto& o : offsets) {
            *o = *o + reinterpret_cast<size_t>(dataBlock);
        }
    }

    std::vector<data_t*> BinaReader::ReadPACV3(bina_t* bina) {
        PACV3Header pacV3Header = *reinterpret_cast<PACV3Header*>(bina);
        data_t* dataBlock = bina;

        char* offsetTable = bina + (pacV3Header.fileSize - pacV3Header.offsetTableSize);

        std::vector<size_t*> offsets = Node::SeekOffsets((unsigned long long)dataBlock, offsetTable, pacV3Header.offsetTableSize);

        // Fix pointers in memory.
        FixPointers(dataBlock, offsets);

        return std::vector<data_t*>({dataBlock + sizeof(PACV3Header)});
    }

    std::vector<data_t*> BinaReader::ReadPACV403(bina_t* bina) {
        PACV403Header* header = reinterpret_cast<PACV403Header*>(bina);
        PACV403MetadataHeader* metaHeader = reinterpret_cast<PACV403MetadataHeader*>(bina + sizeof(PACV403Header));
        
        data_t* dataBlock = bina;
        data_t* pRoot = bina + header->rootOffset;

        char* offsetTable = bina + (header->rootOffset - metaHeader->offsetTableSize);
        std::vector<size_t*> offsets = Node::SeekOffsets((unsigned long long)dataBlock, offsetTable, metaHeader->offsetTableSize);

        // Fix pointers in memory.
        FixPointers(dataBlock, offsets);

        bina_t* uncompressed = new bina_t[header->rootUncompressedSize];
        bina_t* pCurrent = uncompressed;

        unsigned int* chunkTableCount = reinterpret_cast<unsigned int*>(bina + (header->rootOffset - metaHeader->offsetTableSize - metaHeader->stringTableSize - metaHeader->chunkTableSize));
        PACV403ChunkTableEntry* entry = reinterpret_cast<PACV403ChunkTableEntry*>(chunkTableCount + 1);

        for (int i = 0; i < *chunkTableCount; i++) {
            PACV403ChunkTableEntry* e = entry + i;
            LZ4_decompress_safe(pRoot, pCurrent, e->compressedSize, e->uncompressedSize);
            pRoot += e->compressedSize;
            pCurrent += e->uncompressedSize;
        }

        std::ofstream outFile;
        outFile.open("../output/w1f01_trr_heightfield.pac", std::ios::out | std::ios::binary);
        outFile.write(uncompressed, header->rootUncompressedSize);
        outFile.close();

        return ReadPACV3(uncompressed);

    }

    std::vector<data_t*> BinaReader::ReadV1(bina_t* bina) {
        BINAV1Header binaHeader = *(BINAV1Header*)bina;
        data_t* dataBlock = bina + sizeof(binaHeader);

        std::vector<size_t*> offsets = Node::SeekOffsets((unsigned long long)bina + sizeof(binaHeader), bina + sizeof(binaHeader) + binaHeader.offsetTableOffset, binaHeader.offsetTableLength);

        // Fix pointers in memory.
        FixPointers(dataBlock, offsets);

        return std::vector<data_t*>({dataBlock});
    }

    // Direct memory loading.
    std::vector<data_t*> BinaReader::ReadV2(bina_t* bina) {

        BINAHeader binaHeader = *(BINAHeader*)bina;

        std::vector<node_t*> nodes;
        std::vector<node_t*> dataBlocks;

        // Create pointers to each node.
        node_t* current = bina + sizeof(binaHeader);
        for (int i = 0; i < binaHeader.nodeCount; i++) {
            nodes.push_back(current);

            NodeHeader* nh = (NodeHeader*) current;
            unsigned long long dataBlockStart = (unsigned long long) current + sizeof(*nh);

            std::vector<size_t*> offsets = Node::SeekOffsets(current);

            // Fix pointers in memory.
            FixPointers((data_t*)dataBlockStart, offsets);

            dataBlocks.push_back(current + sizeof(NodeHeader));

            current += nh->length;
        }
        return dataBlocks;
    }

}