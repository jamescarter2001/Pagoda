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
        PAC_V3_TYPE_NONE = 0,
        PAC_V3_TYPE_ROOT = 1,
        PAC_V3_TYPE_IS_SPLIT = 2,
        PAC_V3_TYPE_HAS_SPLITS = 4,
        PAC_V3_TYPE_UNKNOWN = 8,
    };

    enum class PACV3Flags : uint16_t {
        PAC_V3_FLAGS_UNKNOWN1 = 8,
        PAC_V3_FLAGS_DEFLATE_COMPRESSED = 0x100U,
        PAC_V3_FLAGS_LZ4_COMPRESSED = 0x200U
    };

    template <class T>
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

    template<class T>
    struct PACV3DepTable {
        uint64_t depCount;
        T* entries;
    };

    template <class T>
    struct PACV3NodeTree {
        unsigned int nodeCount;
        unsigned int dataNodeCount;
        PACV3TreeNode<T>* nodes;
        int32_t* dataNodeIndicies;
    };

    enum class PACV3DataEntryFlags : uint64_t {
        PAC_V3_DATA_ENTRY_FLAGS_REGULAR_FILE = 0,
        PAC_V3_DATA_ENTRY_FLAGS_NOT_HERE = 1,
        PAC_V3_DATA_ENTRY_FLAGS_BINA_FILE = 2
    };

    struct PACV3DataEntry {
        unsigned int uid;
        unsigned int dataSize;
        uint64_t unknown1;
        void* data;
        uint64_t unknown2;
        char* ext;
        PACV3DataEntryFlags flags;
    };

    struct PACV3Header {
        unsigned int signature;  // PACx
        char version[3];         // 301
        char endianFlag;         // B, L
        unsigned int uid;        // Unique identifier for this PAC file.
        unsigned int fileSize;   // Total size, in bytes, of this PAC file.
        unsigned int treesSize;  // Total size, in bytes, of the "trees" block, immediately following this header.
        unsigned int depTableSize;
        unsigned int dataEntriesSize;  // Total size, in bytes, of the data entries block.
        unsigned int stringTableSize;  // Total sizes, in bytes, of the string table block.
        unsigned int fileDataSize;     // Total size, in bytes, of the raw files that have been packed into this PAC file.
        unsigned int offsetTableSize;  // Total size, in bytes, of the offset table.
        PACV3Type type;                // is_root = 0x1, is_split = 0x2, has_splits = 0x4, unknown = 0x8
        PACV3Flags flags;              // unknown = 0x8, deflate_compressed = 0x100U, lz4_compressed = 0x200U
        unsigned int depCount;
    };

    // PACx4

    enum class PACV4Flags : uint16_t {
        PACV4_FLAGS_NONE = 0,
        PACV4_FLAGS_UNKNOWN1 = 1,
        PACV4_FLAGS_HAS_PARENTS = 2,
        PACV4_FLAGS_HAS_METADATA = 8
    };

    struct PAC403ParentInfo {
        char* parentFileName;
    };

    struct PACV403ParentTable {
        uint64_t parentCount;
        PAC403ParentInfo* data;
    };

    struct PACV403ChunkTableEntry {
        uint32_t compressedSize;
        uint32_t uncompressedSize;
    };

    struct PACV403ChunkTable {
        unsigned int chunkCount;
        PACV403ChunkTableEntry entries[];
    };

    struct LZ4DepInfo {
        char* name;
        uint32_t compressedSize;
        uint32_t uncompressedSize;
        uint32_t dataPos;
        unsigned int chunkCount;
        PACV403ChunkTableEntry* entries;
    };

    struct PACV403MetadataHeader {
        uint32_t parentsSize;
        uint32_t chunkTableSize;
        uint32_t stringTableSize;
        uint32_t offsetTableSize;

        PACV403ParentTable* ParentTable() {
            return reinterpret_cast<PACV403ParentTable*>(this + 1);
        }

        PACV403ChunkTable* ChunkTable() {
            return ptradd<PACV403ChunkTable>(ParentTable(), parentsSize);
        }

        char* StringTable() {
            return ptradd<char>(ChunkTable(), chunkTableSize);
        }

        char* OffsetTable() {
            return ptradd<char>(StringTable(), stringTableSize);
        }
    };

    struct PACV403Header {
        uint32_t signature;  // PACx
        char version[3];     // 403
        char endianFlag;     // B, L
        uint32_t uid;        // Unique identifier for this PAC file.
        uint32_t fileSize;   // Total size, in bytes, of this PAC file.
        uint32_t rootOffset; // Offset of the root PAC file.
        uint32_t rootCompressedSize; // Compressed size of the root PAC file.
        uint32_t rootUncompressedSize; // Uncompressed size of the root PAC file.
        PACV4Flags flagsV4;
        PACV3Flags flagsV3;

        PACV403MetadataHeader* MetadataHeader() {
            return reinterpret_cast<PACV403MetadataHeader*>(this + 1);
        }
    };

    inline bool isFlagSet(PACV3Flags src, PACV3Flags flag) {
        return static_cast<uint16_t>(src) & static_cast<uint16_t>(flag);
    }

    inline bool isFlagSet(PACV4Flags src, PACV4Flags flag) {
        return static_cast<uint16_t>(src) & static_cast<uint16_t>(flag);
    }
}