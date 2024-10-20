#include "pgpch.h"
#include "pg_bina_template_converter.h"

namespace Pagoda::Database {
    BINATemplateConverter::BINATemplateConverter(unsigned int ptrSize) : m_ptrSize(ptrSize) {
        this->m_symbolMap = {
            {BINA_SYM_U8,  {sizeof(uint8_t),  [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->u8,  base); }}},
            {BINA_SYM_U16, {sizeof(uint16_t), [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->u16, base); }}},
            {BINA_SYM_U32, {sizeof(uint32_t), [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->u32, base); }}},
            {BINA_SYM_U64, {sizeof(uint64_t), [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->u64, base); }}},
            {BINA_SYM_S8,  {sizeof(int8_t),   [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->s8,  base); }}},
            {BINA_SYM_S16, {sizeof(int16_t),  [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->s16, base); }}},
            {BINA_SYM_S32, {sizeof(int32_t),  [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->s32, base); }}},
            {BINA_SYM_S64, {sizeof(int64_t),  [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->s64, base); }}},
            {BINA_SYM_F32, {sizeof(float),    [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->f32);       }}},
            {BINA_SYM_F64, {sizeof(double),   [](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { std::from_chars(str.data(), str.data() + str.size(), parser->f64);       }}},
            {BINA_SYM_STR, {ptrSize,          [this](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { parser->str = nh.dataBlockSize + nh.stringTablePositionMap.at(str);  }}},
            {BINA_SYM_REF, {ptrSize,          [this](DataParser* const& parser, const NodeHandle& nh, const std::string& str, const int& base) { parser->ref = nh.offsetMap.at(str); }}}
        };
    }
    BINATemplateConverter::~BINATemplateConverter() {}

    bool BINATemplateConverter::IsRefType(const std::string& type) {
        return type == BINA_SYM_STR || type == BINA_SYM_REF;
    }

    bool BINATemplateConverter::IsFloatType(const std::string& type) {
        return type == BINA_SYM_F32 || type == BINA_SYM_F64;
    }

    BINATemplateConverter::NormalizedResult BINATemplateConverter::GetNormalizedData(const std::string& data, const std::string& type) {
        // no-op for float/reference types.
        if (IsFloatType(type) || IsRefType(type)) {
            return {data, 0};
        }

        // Binary
        if (data.rfind("0b") == 0 || data.rfind("0B") == 0) {
            return {data.substr(2), 2};
        }

        // Hex
        if (data.rfind("0x") == 0 || data.rfind("0X") == 0) {
            return {data.substr(2), 16};
        }

        // Decimal
        return {data, 10};
    }

    void BINATemplateConverter::ByteSwap(uint8_t* const buffer, const size_t size) {
        for (size_t i = 0; i < size / 2; ++i) {
            // Swap bytes between the i-th element and the (size - i - 1)-th element
            std::swap(buffer[i], buffer[size - i - 1]);
        }
    };

    size_t BINATemplateConverter::GetSizeOfType(std::string type) {
        return m_symbolMap.at(type).size;
    }

    void BINATemplateConverter::Write(char** dst, void* data, std::string type) {
        size_t typeSize = GetSizeOfType(type);
        memcpy(*dst, data, typeSize);
        *dst += typeSize;
    }

    void BINATemplateConverter::WriteData(char** ppCurrentOffset, const NodeHandle& nh, std::string type, std::string data, const EndianType& endianType) {
        DataParser p = {};
        const SymbolHandle& h = m_symbolMap.at(type);

        NormalizedResult r = GetNormalizedData(data, type);
        h.parser(&p, nh, r.str, r.base);

        if (endianType == EndianType::ENDIAN_TYPE_BIG) {
            ByteSwap((uint8_t*)&p.ref, h.size);
        }

        this->Write(ppCurrentOffset, &p.u64, type);
    }

    void BINATemplateConverter::ConvertTemplateAndSave(const char src[], const char dest[], const EndianType& endianType) {
        const NodeHandle handle = this->ReadTemplate(src);

        std::ifstream srcFile(src, std::ios::in);
        unsigned int count = 0;

        const size_t fileSize = handle.nodeSize + sizeof(BINAHeader);
        char* outHeap = new char[fileSize];
        memset(outHeap, 0, fileSize);

        BINAHeader* bh = (BINAHeader*)outHeap;
        NodeHeader* nh = (NodeHeader*)(outHeap + sizeof(BINAHeader));

        bh->header = binaSig;
        memcpy(bh->version, binaVer, 3);
        bh->endianFlag = 'L';
        bh->fileSize = (unsigned int)fileSize;
        bh->nodeCount = 1;

        nh->signature = dataSig;
        nh->length = (unsigned int)(handle.nodeSize);
        nh->stringTableOffset = (unsigned int)handle.dataBlockSize;
        nh->stringTableLength = (unsigned int)(handle.stringTable.str().size());
        nh->offsetTableLength = (unsigned int)(handle.offsetTable.str().size());
        nh->additionalDataLength = ADD_DATA_LENGTH;

        char* pCurrentOffset = (char*)nh + sizeof(NodeHeader);

        try {
            std::string line;
            while (std::getline(srcFile, line)) {
                if (line.at(0) == '\t') {
                    std::string entry = line.substr(1, line.size() - 1);

                    std::vector<std::string> fragments = Base::Strings::Split(entry, " ");
                    if (fragments.size() < 2) {
                        continue;
                    }
                    for (int i = 1; i < fragments.size(); i++) {
                        this->WriteData(&pCurrentOffset, handle, fragments[0], fragments[i], endianType);
                    }
                }
            }

            memcpy(pCurrentOffset, handle.stringTable.str().c_str(), handle.stringTable.str().size());
            pCurrentOffset += handle.stringTable.str().size();
            memcpy(pCurrentOffset, handle.offsetTable.str().c_str(), handle.offsetTable.str().size());

            if (endianType == EndianType::ENDIAN_TYPE_BIG) {
                SwapBINAHeader(bh);
                SwapNodeHeader(nh);
            }

            std::ofstream outFile(dest, std::ios::out | std::ios::binary);
            outFile.write(outHeap, bh->fileSize);
            outFile.close();

        } catch (...) {
            std::cout << "Invalid template file." << std::endl;
        }
        delete[] outHeap;
    }

    BINATemplateConverter::NodeHandle BINATemplateConverter::ReadTemplate(const char src[]) {
        NodeHandle handle = {};
        std::ifstream srcFile(src, std::ios::in);

        std::string line;
        unsigned long long lastOffset = 0;
        while (std::getline(srcFile, line)) {
            if (line.at(0) != '\t') {
                std::vector<std::string> sectionFragments = Base::Strings::Split(line, ":");
                if (sectionFragments.size() == 2) {
                    handle.offsetMap.insert({sectionFragments[0], (unsigned int)handle.dataBlockSize});
                }
                continue; 
            }

            std::string entry = line.substr(1, line.size() - 1);
            std::vector<std::string> fragments = Base::Strings::Split(entry, " ");

            // Ignore blank lines.
            if (fragments.size() < 2) { continue; }

            for (int i = 1; i < fragments.size(); i++) {
                // If string type, append to string table.
                if (fragments[0] == BINA_SYM_STR) {
                    auto stringEntry = handle.stringTablePositionMap.find(fragments[i]);

                    // Is this the first time seeing this string?
                    if (stringEntry == handle.stringTablePositionMap.end()) {
                        handle.stringTablePositionMap.insert({fragments[i], (unsigned int)handle.stringTable.str().size()});

                        handle.stringTable << fragments[i] << '\0';
                    }
                }

                // If pointer type, append to offsets.
                if (fragments[0] == BINA_SYM_STR || fragments[0] == BINA_SYM_REF) {
                    handle.offsets.push_back(handle.dataBlockSize - lastOffset);
                    lastOffset = handle.dataBlockSize;
                }

                // Append to data block size.
                handle.dataBlockSize += GetSizeOfType(fragments[0]);
            }
        }
        srcFile.close();

        handle.offsetTable = DatabaseUtils::GenerateBINAOffsetTable(handle.offsets);

        size_t nodeSize = sizeof(NodeHeader) +
                          handle.dataBlockSize +
                          handle.stringTable.str().size() +
                          handle.offsetTable.str().size();

        // Align tables to 4 bytes.
        nodeSize += DatabaseUtils::Align(handle.stringTable);
        nodeSize += DatabaseUtils::Align(handle.offsetTable, 0x10, nodeSize);

        handle.nodeSize = nodeSize;

        return handle;
    }
}