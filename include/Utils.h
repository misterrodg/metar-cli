#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::string join_strings(
    const std::vector<std::string>& strings,
    const std::string& delimiter
);

std::vector<std::string> split_strings(
    const std::string& string,
    const char& delimiter
);

void capitalize(std::string& s);

float parse_fractional_number(const std::string& s);

#endif
