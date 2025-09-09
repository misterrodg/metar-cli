#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <sstream>

std::string join_strings(
    const std::vector<std::string>& strings,
    const std::string& delimiter
) {
    if (strings.empty()) {
        return "";
    }

    std::stringstream ss;
    for (size_t i = 0; i < strings.size(); ++i) {
        ss << strings[i];
        if (i < strings.size() - 1) {
            ss << delimiter;
        }
    }
    return ss.str();
}

void capitalize(std::string& s) {
    std::transform(
        s.begin(),
        s.end(),
        s.begin(),
        [](unsigned char c){ return std::toupper(c); });
}
