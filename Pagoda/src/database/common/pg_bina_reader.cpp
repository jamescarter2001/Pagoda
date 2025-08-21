#include "pgpch.h"

#include "pg_bina_reader.h"

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

        if (pacv4Header.signature == pacSig && (VersionEquals(pacv4Header.version, "403") || VersionEquals(pacv4Header.version, "405"))) {
            std::cout << "Reading PAC: " << std::string(pacv4Header.version, pacv4Header.version + 3) << std::endl;
            bina = new bina_t[pacv4Header.fileSize];
            file.read(bina, pacv4Header.fileSize);

            return ReadPACV403(bina, path.replace(path.find(".pac"), sizeof(".pac") - 1, ".inf.pac"));
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

        std::vector<size_t*> offsets = Node::SeekOffsets(dataBlock, offsetTable, pacV3Header.offsetTableSize);

        // Fix pointers in memory.
        FixPointers(dataBlock, offsets);

        return std::vector<data_t*>({dataBlock + sizeof(PACV3Header)});
    }

    std::vector<data_t*> BinaReader::ReadPACV403(bina_t* bina, const std::string& fileName) {
        PACV403Header* header = reinterpret_cast<PACV403Header*>(bina);
        PACV403MetadataHeader* metaHeader = header->MetadataHeader();
        
        data_t* dataBlock = bina;

        char* offsetTable = metaHeader->OffsetTable();
        std::vector<size_t*> offsets = Node::SeekOffsets(dataBlock, offsetTable, metaHeader->offsetTableSize);

        // Fix pointers in memory.
        FixPointers(dataBlock, offsets);

        PACV403ParentTable* parentTable = metaHeader->ParentTable();
        std::vector<PAC403ParentInfo> parents(parentTable->data, parentTable->data + parentTable->parentCount);

        PACV403ChunkTable* chunkTable = reinterpret_cast<PACV403ChunkTable*>(bina + sizeof(PACV403Header) + sizeof(PACV403MetadataHeader) + metaHeader->parentsSize);

        pac_t* pUncompressedRoot = PACV4Decompress(bina + header->rootOffset, header->rootUncompressedSize, chunkTable->entries, chunkTable->chunkCount);

        PACV3Header* rootHeader = reinterpret_cast<PACV3Header*>(pUncompressedRoot);
        char* rootOffsetTable = pUncompressedRoot + (rootHeader->fileSize - rootHeader->offsetTableSize);
        std::vector<size_t*> rootOffsets = Node::SeekOffsets(pUncompressedRoot, rootOffsetTable, rootHeader->offsetTableSize);
        FixPointers(pUncompressedRoot, rootOffsets);

        std::vector<pac_t*> pacs;
        pacs.push_back(pUncompressedRoot);
        PACV3DepTable<LZ4DepInfo>* depTable = reinterpret_cast<PACV3DepTable<LZ4DepInfo>*>(pUncompressedRoot + sizeof(PACV3Header) + rootHeader->treesSize);

        for (int i = 0; i < rootHeader->depCount; i++) {
            LZ4DepInfo* di = depTable->entries + i;
            pac_t* pUncompressedSplit = PACV4Decompress(bina + di->dataPos, di->uncompressedSize, di->entries, di->chunkCount);
            pacs.push_back(pUncompressedSplit);
        }

        int fileIndex = 0;
        for (const auto& p : pacs) {
            PACV3Header* pacV3Header = reinterpret_cast<PACV3Header*>(p);
            std::ofstream outFile;
            outFile.open(fileName + "." + std::to_string(fileIndex++), std::ios::out | std::ios::binary);
            outFile.write(p, pacV3Header->fileSize);
            outFile.close();
        }

        return ReadPACV3(pUncompressedRoot);

    }

    std::vector<data_t*> BinaReader::ReadV1(bina_t* bina) {
        BINAV1Header binaHeader = *reinterpret_cast<BINAV1Header*>(bina);
        data_t* dataBlock = bina + sizeof(binaHeader);

        std::vector<size_t*> offsets = Node::SeekOffsets(bina + sizeof(binaHeader), bina + sizeof(binaHeader) + binaHeader.offsetTableOffset, binaHeader.offsetTableLength);

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

    pac_t* BinaReader::PACV4Decompress(pac_t* const pCompressedPac, const unsigned int pacUncompressedSize, PACV403ChunkTableEntry* const chunkTable, const unsigned int chunkCount) {
        pac_t* pUncompressedPac = new pac_t[pacUncompressedSize];

        pac_t* pCompressedCursor = pCompressedPac;
        pac_t* pUncompressedCursor = pUncompressedPac;

        for (int i = 0; i < chunkCount; i++) {
            PACV403ChunkTableEntry* e = chunkTable + i;
            LZ4_decompress_safe(pCompressedCursor, pUncompressedCursor, e->compressedSize, e->uncompressedSize);
            pCompressedCursor += e->compressedSize;
            pUncompressedCursor += e->uncompressedSize;
        }

        return pUncompressedPac;
    }

}