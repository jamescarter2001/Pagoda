#include "pgpch.h"
#include "pg_strings.h"

namespace Pagoda::Base {
    std::vector<std::string> Strings::Split(std::string str, std::string delim) {
        size_t strSize = str.size();
        std::vector<std::string> fragments;

        size_t pos = 0;
        std::string token;
        while ((pos = str.find(delim)) != std::string::npos) {
            token = str.substr(0, pos);
            fragments.push_back(token);
            str.erase(0, pos + delim.length());
        }

        if (str.substr(0, strSize - pos).find(" ") == std::string::npos) {
            fragments.push_back(str.substr(0, strSize - pos));
        }

        return fragments;
    }

    std::wstring Strings::STR_TO_WSTR(std::string& s) {
        return std::wstring(s.begin(), s.end());
    }
}