#pragma once
#include "pgpch.h"
#include "pg_bina_raw.h"

namespace Pagoda::Database {

	// PACx2

    struct PACV2Header {
        unsigned int signature;
        unsigned int blockSize;
        unsigned int dataEntriesSize;
        unsigned int proxiesSize;
        unsigned int stringTableSize;
        unsigned int offsetTableSize;
        uint8_t unknown1;
        uint8_t padding1;
        uint16_t padding2;
    };

    // PACx3

    enum class PACV3Type : uint16_t {
        PAC_V3_ROOT = 0x1,
        PAC_V3_IS_SPLIT = 0x2,
        PAC_V3_HAS_SPLITS = 0x4,
        PAC_V3_UNKNOWN = 0x8,
    };

    struct PACV3Header {
        unsigned int signature;        // PACx
        char version[3];               // 301
        char endianFlag;               // B, L
        unsigned int uid;              // Unique identifier for this PAC file.
        unsigned int fileSize;         // Total size, in bytes, of this PAC file.
        unsigned int treesSize;        // Total size, in bytes, of the "trees" block, immediately following this header.
        unsigned int depTableSize;
        unsigned int dataEntriesSize;  // Total size, in bytes, of the data entries block.
        unsigned int stringTableSize;  // Total sizes, in bytes, of the string table block.
        unsigned int fileDataSize;     // Total size, in bytes, of the raw files that have been packed into this PAC file.
        unsigned int offsetTableSize;  // Total size, in bytes, of the offset table.
        PACV3Type type;                // is_root = 0x1, is_split = 0x2, has_splits = 0x4, unknown = 0x8
        unsigned short flags;          // unknown = 0x8, deflate_compressed = 0x100U, lz4_compressed = 0x200U
        unsigned int depCount;
    };

    template <typename T>
    struct PACV3TreeNode {
        char* name;
        T* data;
        int32_t* childIndicies;
        int32_t parentIndex;
        int32_t globalIndex;
        int32_t dataIndex;
        uint16_t childCount;
        uint8_t hasData;
        uint8_t bufStartIndex;
    };

    struct PACV3NodeTree {
        unsigned int nodeCount;
        unsigned int dataNodeCount;
        PACV3TreeNode<void>* nodes;
        int32_t* dataNodeIndicies;
    };

    struct PACV3DataEntry {
        unsigned int uid;
        unsigned int dataSize;
        uint64_t unknown1;
        void* data;
        uint64_t unknown2;
        char* ext;
    };

    // PACx4

    struct PACV403Header {
        uint32_t signature;      // PACx
        char version[3];         // 403
        char endianFlag;         // B, L
        uint32_t uid;            // Unique identifier for this PAC file.
        uint32_t fileSize;       // Total size, in bytes, of this PAC file.
        uint32_t rootOffset;
        uint32_t rootCompressedSize;
        uint32_t rootUncompressedSize;
        uint16_t flagsV4;
        uint16_t flagsV3;
    };

    struct PACV403MetadataHeader {
        uint32_t parentsSize;
        uint32_t chunkTableSize;
        uint32_t stringTableSize;
        uint32_t offsetTableSize;
    };

    struct PACV403ChunkTableEntry {
        uint32_t compressedSize;
        uint32_t uncompressedSize;
    };
}