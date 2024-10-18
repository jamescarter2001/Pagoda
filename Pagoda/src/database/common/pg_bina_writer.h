#pragma once
#include "base/collection/pg_ordered_map.h"

#include "database/common/pg_bina.h"

#include "database/util/pg_database_util.h"

namespace Pagoda::Database {
    enum StructHandleType {
        STRUCT_HANDLE_TYPE_STD, // For standard BINA structs.
        STRUCT_HANDLE_TYPE_STR, // For strings.
        STRUCT_HANDLE_TYPE_MSC // For miscellaneous, unstructured data stored between the string table and the offset table (e.g. raw files in a PACx3 file).
    };

    enum OffsetType {
        OFFSET_TYPE_RELATIVE,
        OFFSET_TYPE_ABSOLUTE
    };

    struct StructHandle {
        const void* pStruct;
        bool writable;
        size_t pOffset;
        size_t size;
        size_t alignedSize;
        StructHandleType type;
    };

    struct NodeInfo {
        OffsetType offsetType;

        Base::OrderedMap<const void*, StructHandle> structs;
        std::stringstream offsetTable;

        size_t length = 0;
        size_t structsSize = 0;
        size_t miscSize = 0;
        size_t stringTableSize = 0;
        size_t offsetTableSize = 0;
    };

    /**
     * Utility class for writing structs directly to binary container (BINA) files.
     */
    class BinaWriter {
    public:
        BinaWriter();
        ~BinaWriter();

        void BeginNode();
        void BeginNode(const OffsetType& offsetType);
        void EndNode();

        void AddString(const char* const str);
        void AddStruct(const void* const structData, const size_t size);
        void AddMisc(const void* const structData, const size_t size);

        void AddStringVector(const std::vector<char*>& structData);

        template<typename T>
        void AddStructVector(const std::vector<T*>& structData) {
            this->AddStruct(&structData[0], sizeof(T*) * (unsigned int)structData.size());

            for (const auto& s : structData) {
                this->AddStruct(s, sizeof(T));
            }
        };

        void Write(const char filePath[]);

    protected:
        virtual void WriteFileHeader(char** const offset, const size_t fileSize, const size_t nodeCount);
        virtual void WriteNodeHeader(char** const offset, const NodeInfo* const handle);

        virtual inline size_t GetFileHeaderSize() {
            return sizeof(BINAHeader);
        }
        virtual inline size_t GetNodeHeaderSize() {
            return sizeof(NodeHeader);
        }
    private:
        static std::vector<size_t> SeekOffsets(const NodeInfo* const nodeInfo);
        static void FixPointers(char* const pNodeBody, const NodeInfo* const nodeInfo, const size_t fromOffset);

        static size_t GetAlignment(const size_t count, const unsigned int factor);

        void WriteData(char** const offset, const Base::OrderedMap<const void*, StructHandle>& structs, const StructHandleType& handleType);
        void WriteData(char** const offset, const Base::OrderedMap<const void*, StructHandle>& structs, const StructHandleType& handleType, const size_t padding);

        std::vector<NodeInfo*> m_nodes;
        NodeInfo* m_activeNode = nullptr;
    };
}