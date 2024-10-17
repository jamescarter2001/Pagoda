#pragma once
#include "base/collection/pg_ordered_map.h"

#include "database/common/pg_bina.h"

#include "database/util/pg_database_util.h"

namespace Pagoda::Database {
    enum StructHandleType {
        STRUCT_HANDLE_TYPE_STD,
        STRUCT_HANDLE_TYPE_STR
    };

    struct StructHandle {
        void* pStruct;
        bool writable;
        size_t pOffset;
        size_t size;
        size_t alignedSize;
        StructHandleType type;
    };

    /**
     * Utility class for writing structs directly to binary container (BINA) files.
     */
    class BinaWriter {
    public:
        void AddString(char* str);
        void AddStruct(void* structData, size_t size);

        void AddStringVector(std::vector<char*>& structData);

        template<typename T>
        void AddStructVector(std::vector<T*>& structData) {
            this->AddStruct(&structData[0], sizeof(T*) * (unsigned int)structData.size());

            for (T* s : structData) {
                this->AddStruct(s, sizeof(T));
            }
        };

        void Write(const char filePath[]);
    private:
        void WriteData(char** offset, const Base::OrderedMap<void*, StructHandle>& structs, const StructHandleType& handleType);
        void FixPointers(char* nodeBody);
        size_t GetAlignment(size_t count, unsigned int factor);

        Base::OrderedMap<void*, StructHandle> m_structs;

        std::vector<unsigned long long> m_offsets;

        size_t m_structSize = 0;
        size_t m_stringTableSize = 0;
    };
}