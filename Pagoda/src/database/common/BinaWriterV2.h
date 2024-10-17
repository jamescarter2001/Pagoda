#pragma once

#include "pgpch.h"

#include "database/common/pg_bina.h"

#include "database/util/pg_database_util.h"


namespace Pagoda::Database {
    enum StructHandleType {
        STRUCT_HANDLE_TYPE_STANDARD,
        STRUCT_HANDLE_TYPE_STR
    };

    struct StructHandle {
        void* pStruct;
        size_t offset;
        size_t size;
        size_t padding;
        StructHandleType handleType;
    };

    class BinaWriterV2 {

    public:
        BinaWriterV2();
        ~BinaWriterV2();

        void AddStruct(void* const pStruct, const size_t size);
        void AddString(char* const pStr);

        void Write(std::string& destFile);

    private:
        StructHandle CreateHandle(void* const pStruct, const size_t offset, const size_t size, const size_t padding, const StructHandleType handleType);
        void AddStruct(const StructHandle h);

        size_t GetAlignment(size_t count, uint32_t factor);

        size_t m_currentOffset = 0;
        size_t m_strCurrentOffset = 0;

        std::vector<StructHandle> m_structs;
        std::vector<StructHandle>::iterator m_structInsert;

        size_t m_structsSize = 0;
        size_t m_stringsSize = 0;
    };
}