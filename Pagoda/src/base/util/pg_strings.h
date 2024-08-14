#pragma once
#include "pgpch.h"

namespace Pagoda::Base {
    class Strings {
    public:
        /*
        * Splits a string against a provided delimeter into a vector of fragments.
        */
        static std::vector<std::string> Split(std::string str, std::string delim);

        /*
        * Converts a given string to a wide string.
        */
        static std::wstring STR_TO_WSTR(std::string& s);
    };
}