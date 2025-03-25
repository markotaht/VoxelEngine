#pragma once
#include <string>
#include <algorithm>

inline bool ends_with(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
        std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

inline bool ends_with(const std::string& str, const char* suffix) {
    return ends_with(str, std::string(suffix));
}