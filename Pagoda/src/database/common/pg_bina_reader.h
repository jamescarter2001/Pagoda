#pragma once
#include "pgpch.h"

#include "database/common/pg_bina.h"

#define READ_BYTES(f, s, n) f.read((char*)s, n)
#define READ_STRUCT(f, s) f.read((char*)&s, sizeof(s))

namespace Pagoda::Database {
    class BinaReader {
    public:
        BinaReader();
        virtual ~BinaReader();

        void FixPointers(data_t* dataBlock, const std::vector<size_t*>& offsets);

        std::vector<data_t*> Read(std::string path);
        std::vector<data_t*> ReadV1(bina_t* bina);
        std::vector<data_t*> ReadV2(bina_t* bina);

        std::vector<data_t*> ReadPACV3(bina_t* bina);
        std::vector<data_t*> ReadPACV403(bina_t* bina);
    };
}