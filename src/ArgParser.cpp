#include "ArgParser.h"
#include "Patterns.h"
#include "RegexUtils.h"
#include "Utils.h"

#include <regex>

ArgParser::ArgParser(int argc, char* const argv[]) : should_translate_(false) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        to_uppercase(arg);

        if (arg == "--TRANSLATE" || arg == "-T") {
            should_translate_ = true;
            continue;
        }

        if (check_airport(arg)) {
            airports_.push_back(arg);
        } else {
            invalid_args_.push_back(arg);
        }
    }
}

bool ArgParser::should_translate() const {
    return should_translate_;
}

const std::vector<std::string>& ArgParser::get_airports() const {
    return airports_;
}

std::string ArgParser::get_airport_string() const {
    return join_strings(airports_, ",");
}

const std::vector<std::string>& ArgParser::get_invalid_args() const {
    return invalid_args_;
}

bool ArgParser::has_invalid_args() const {
    return !invalid_args_.empty();
}

std::string ArgParser::get_invalid_arg_string() const {
    return join_strings(invalid_args_, ",");
}

bool ArgParser::check_airport(const std::string& airport_string) {
    const std::regex station_regex =
        RegexUtils::make_exact_regex(Patterns::STATION);
    if (std::regex_match(airport_string, station_regex)) {
        return true;
    }
    return false;
}
