#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
void clearInputStream();
bool isBlank(const std::string& value);
std::string trim(const std::string& value);
}

#endif
