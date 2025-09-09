#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>

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

std::vector<std::string> split_strings(
    const std::string& string,
    const char& delimiter
) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(string);

    while(std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void capitalize(std::string& s) {
    std::transform(
        s.begin(),
        s.end(),
        s.begin(),
        [](unsigned char c){ return std::toupper(c); });
}

float parse_fractional_number(const std::string &s) {
    std::istringstream iss(s);
    int whole = 0;
    int num = 0;
    int den = 1;

    if (iss >> whole) {
        if (iss.peek() == ' ') {
            iss.get();
            if (iss >> num && iss.get() == '/' && iss >> den && den != 0) {
                return static_cast<float>(whole) + static_cast<float>(num) / den;
            }
        }
        return static_cast<float>(whole);
    }

    iss.clear();
    iss.str(s);
    if (iss >> num && iss.get() == '/' && iss >> den && den != 0) {
        return static_cast<float>(num) / den;
    }

    return -1;
}
