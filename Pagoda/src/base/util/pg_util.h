#pragma once
#include "pgpch.h"

static std::wstring STR_TO_WSTR(std::string& s) {
    return std::wstring(s.begin(), s.end());
}