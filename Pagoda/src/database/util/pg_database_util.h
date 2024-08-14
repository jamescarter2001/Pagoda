#pragma once
#include "pgpch.h"

// Helper functions for database library operations.

namespace Pagoda::Database {
    class DatabaseUtils {
    public:
        // Calculate alignment factor for BINA contents.
        static unsigned int GetAlignment(size_t count, unsigned int factor = 4);

        // Calculate BINA offset table.
        static std::stringstream GenerateBINAOffsetTable(std::vector<unsigned long long> offsets);

        // Align stringstream data.
        static void Align(std::stringstream& ss);
    };
}