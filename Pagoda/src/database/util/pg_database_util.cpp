#include "pgpch.h"
#include "pg_database_util.h"

namespace Pagoda::Database {
    size_t DatabaseUtils::GetAlignment(const size_t count, const size_t factor) {
        size_t val = factor - (count % factor);
        return val == factor ? 0 : val;
    }

    std::stringstream DatabaseUtils::GenerateBINAOffsetTable(const std::vector<size_t>& offsets) {
        std::stringstream offsetTableStream;
        for (const auto& o : offsets) {
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

    size_t DatabaseUtils::Align(std::stringstream& ss, const size_t factor) {
        return Align(ss, factor, ss.str().size());
    };

    size_t DatabaseUtils::Align(std::stringstream& ss, const size_t factor, const size_t size) {
        size_t alignment = DatabaseUtils::GetAlignment(size, factor);
        for (unsigned int i = 0; i < alignment; i++) {
            ss << '\0';
        }

        return alignment;
    }
}