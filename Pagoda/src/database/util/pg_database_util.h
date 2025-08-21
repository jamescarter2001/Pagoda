#pragma once
#include "pgpch.h"

// Helper functions for database library operations.

namespace Pagoda::Database {
    class DatabaseUtils {
    public:
        // Calculate alignment factor for BINA contents.
        static size_t GetAlignment(const size_t count, const size_t factor = 0x4U);

        // Calculate BINA offset table.
        static std::stringstream GenerateBINAOffsetTable(const std::vector<size_t>& offsets);

        // Align stringstream data.
        static size_t Align(std::stringstream& ss, const size_t factor = 0x4U);
        static size_t Align(std::stringstream& ss, const size_t factor, const size_t size);
    };
}