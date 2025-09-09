#include "ArgParser.h"
#include "Utils.h"

#include <regex>
#include <stdio.h>


ArgParser::ArgParser(int argc, char* argv[]) {
    m_use_translation = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        capitalize(arg);

        if (arg == "--TRANSLATE") {
            m_use_translation = true;
            continue;
        }

        if (check_airport(arg)) {
            m_airports.push_back(arg);
        }
    }
}

bool ArgParser::get_use_translation() const {
    return m_use_translation;
}

std::string ArgParser::get_airport_string() const {
    return join_strings(m_airports, ",");
}

bool ArgParser::check_airport(const std::string& airport_string) {
    std::regex airport_pattern("K[A-Z0-9]{3}");
    if (std::regex_match(airport_string, airport_pattern)) {
        return true;
    }
    return false;
}
