#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace {
bool isWhitespace(unsigned char ch) {
    return std::isspace(ch) != 0;
}
}

namespace Utils {
void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

bool isBlank(const std::string& value) {
    return std::all_of(value.begin(), value.end(), [](unsigned char ch) { return isWhitespace(ch); });
}

std::string trim(const std::string& value) {
    const auto begin = std::find_if_not(value.begin(), value.end(),
                                        [](unsigned char ch) { return isWhitespace(ch); });
    if (begin == value.end()) {
        return "";
    }

    const auto end =
        std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) { return isWhitespace(ch); }).base();
    return std::string(begin, end);
}
}
