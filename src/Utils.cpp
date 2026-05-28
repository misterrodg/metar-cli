#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>

std::string join_strings(const std::vector<std::string>& inputs,
                         std::string_view delimiter) {
    if (inputs.empty()) {
        return "";
    }

    std::ostringstream oss;
    for (size_t i = 0; i < inputs.size(); ++i) {
        oss << inputs[i];
        if (i + 1 < inputs.size()) {
            oss << delimiter;
        }
    }
    return oss.str();
}

std::vector<std::string> split_strings(const std::string& input,
                                       char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(input);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::string> split_on_whitespace(std::string_view input) {
    std::vector<std::string> result;
    size_t i = 0;
    while (i < input.size()) {
        while (i < input.size() &&
               std::isspace(static_cast<unsigned char>(input[i])))
            ++i;
        if (i >= input.size())
            break;
        size_t j = i;
        while (j < input.size() &&
               !std::isspace(static_cast<unsigned char>(input[j])))
            ++j;
        result.emplace_back(input.substr(i, j - i));
        i = j;
    }
    return result;
}

void to_uppercase(std::string& input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c) { return std::toupper(c); });
}

std::optional<double> parse_fractional_number(const std::string& input) {
    std::istringstream iss(input);

    int whole = 0;
    int num = 0;
    int den = 1;
    char slash = '\0';

    if (iss >> whole) {
        iss >> std::ws;

        if (iss.eof()) {
            return static_cast<double>(whole);
        }

        if ((iss >> num) && (iss >> slash) && slash == '/' && (iss >> den) &&
            den != 0) {
            iss >> std::ws;
            if (iss.eof()) {
                return static_cast<double>(whole) +
                       static_cast<double>(num) / den;
            }
        }
        return std::nullopt;
    }

    iss.clear();
    iss.str(input);

    if ((iss >> num) && (iss >> slash) && slash == '/' && (iss >> den) &&
        den != 0) {
        iss >> std::ws;
        if (iss.eof()) {
            return static_cast<double>(num) / den;
        }
    }

    return std::nullopt;
}

void expect(bool ok, const char* msg) {
    if (!ok)
        throw std::runtime_error(msg);
}
