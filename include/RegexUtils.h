#ifndef REGEX_UTILS_H
#define REGEX_UTILS_H

#include <regex>
#include <string>
#include <string_view>

namespace RegexUtils {

inline std::regex make_regex(std::string_view pattern) {
    return std::regex(std::string(pattern));
}

inline std::regex make_exact_regex(std::string_view pattern) {
    return std::regex("^" + std::string(pattern) + "$");
}

inline std::regex make_token_regex(std::string_view pattern) {
    return std::regex(R"(\b)" + std::string(pattern) + R"(\b)");
}

inline std::regex make_start_token_regex(std::string_view pattern) {
    return std::regex("^(" + std::string(pattern) + R"()\b)");
}

} // namespace RegexUtils

#endif
