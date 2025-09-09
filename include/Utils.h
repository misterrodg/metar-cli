#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::string join_strings(
    const std::vector<std::string>& strings,
    const std::string& delimiter
);

void capitalize(std::string& s);

#endif
