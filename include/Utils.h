#ifndef UTILS_H
#define UTILS_H

#include <optional>
#include <string>
#include <string_view>
#include <vector>

std::string join_strings(const std::vector<std::string>& inputs,
                         std::string_view delimiter);

std::vector<std::string> split_strings(const std::string& input,
                                       char delimiter);

std::vector<std::string> split_on_whitespace(std::string_view input);

void to_uppercase(std::string& input);

std::optional<double> parse_fractional_number(const std::string& input);

void expect(bool ok, const char* msg);

#endif
