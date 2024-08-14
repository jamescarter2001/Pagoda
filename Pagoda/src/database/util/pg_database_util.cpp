#include "pgpch.h"
#include "pg_database_util.h"

namespace Pagoda::Database {
    unsigned int DatabaseUtils::GetAlignment(size_t count, unsigned int factor) {
        size_t val = factor - (count % factor);

        // Is the string already aligned?
        if (val == factor) {
            return 0;
        }

        return (unsigned int)val;
    }

    std::stringstream DatabaseUtils::GenerateBINAOffsetTable(std::vector<unsigned long long> offsets) {
        std::stringstream offsetTableStream;
        for (unsigned long long o : offsets) {
            if (o > 0xFFFC) {
                uint32_t val = ((uint32_t)o >> 2) | 0xC0000000;
                char* bytes = (char*)&val;
                offsetTableStream << bytes[3] << bytes[2] << bytes[1] << bytes[0];
            } else if (o > 0xFC) {
                uint16_t val = ((uint16_t)o >> 2) | 0x8000;
                char* bytes = (char*)&val;
                offsetTableStream << bytes[1] << bytes[0];
            } else {
                uint8_t val = ((uint8_t)o >> 2) | 0x40;
                offsetTableStream << val;
            }
        }

        offsetTableStream << '\0';

        return offsetTableStream;
    }

    void DatabaseUtils::Align(std::stringstream& ss) {
        unsigned int alignment = DatabaseUtils::GetAlignment(ss.str().size());
        for (unsigned int i = 0; i < alignment; i++) {
            ss << '\0';
        }
    }
}