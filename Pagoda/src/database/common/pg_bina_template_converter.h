#pragma once
#include "pgpch.h"

#include "base/functional/pg_functional.h"

#include "base/util/pg_strings.h"

#include "database/util/pg_database_util.h"

#include "database/common/pg_bina.h"
#include "database/common/pg_bina_raw.h"

#define BINA_SYM_U8  "u8"
#define BINA_SYM_U16 "u16"
#define BINA_SYM_U32 "u32"
#define BINA_SYM_U64 "u64"
#define BINA_SYM_S8  "s8"
#define BINA_SYM_S16 "s16"
#define BINA_SYM_S32 "s32"
#define BINA_SYM_S64 "s64"
#define BINA_SYM_F32 "f32"
#define BINA_SYM_F64 "f64"
#define BINA_SYM_STR "str"
#define BINA_SYM_REF "ref"

#define PTR_SIZE_32 4
#define PTR_SIZE_64 8

namespace Pagoda::Database {
    class BINATemplateConverter {
    public:
        enum class EndianType {
            ENDIAN_TYPE_BIG,
            ENDIAN_TYPE_LITTLE
        };

        BINATemplateConverter(unsigned int ptrSize);
        virtual ~BINATemplateConverter();

        void ConvertTemplateAndSave(const char src[], const char dest[], const EndianType& endianType = EndianType::ENDIAN_TYPE_LITTLE);

    private:
        union DataParser {
            float f32;
            double f64;
            int8_t s8;
            int16_t s16;
            int32_t s32;
            int64_t s64;
            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            uint64_t u64 = 0;
            size_t ref;
            size_t str;
        };

        struct NodeHandle {
            std::stringstream offsetTable;
            std::map<std::string, unsigned int> offsetMap;
            std::vector<unsigned long long> offsets;

            std::stringstream stringTable;
            std::map<std::string, unsigned int> stringTablePositionMap;

            size_t dataBlockSize = 0;
            size_t nodeSize = 0;
        };

        using Parser = std::function<void(DataParser* const&, const NodeHandle&, const std::string&, const int&)>;

        struct SymbolHandle {
            size_t size = 0;
            Parser parser = [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) {};
        };

        struct NormalizedResult {
            std::string str;
            int base;
        };

        bool IsRefType(const std::string& type);
        bool IsFloatType(const std::string& type);

        NormalizedResult GetNormalizedData(const std::string& data, const std::string& type);
        void ByteSwap(uint8_t* const buffer, const size_t size);

        size_t GetSizeOfType(std::string type);

        void Write(char** ppDest, void* pSrc, std::string type);
        void WriteData(char** ppCurrentOffset, const NodeHandle& nh, std::string type, std::string data, const EndianType& endianType);

        NodeHandle ReadTemplate(const char src[]);

        unsigned int m_ptrSize = 0;

        std::unordered_map<std::string, SymbolHandle> m_symbolMap;
    };
}