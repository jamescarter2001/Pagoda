#pragma once
#include "pgpch.h"

#include "database/common/pg_bina_raw.h"

typedef char pac_t;
typedef char bina_t;
typedef char node_t;
typedef char data_t;

namespace Pagoda::Database {
    class Node {
    public:
        static std::vector<size_t*> SeekOffsets(data_t* const start, char* offsetTable, unsigned int offsetTableLength);
        static std::vector<size_t*> SeekOffsets(node_t* node);

        static std::vector<size_t*> PrintOffsets(data_t* const start, char* offsetTable, unsigned int offsetTableLength);
    };
}