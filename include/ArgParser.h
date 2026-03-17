#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>
#include <vector>

class ArgParser {
public:
    ArgParser(int argc, char* const argv[]);

    bool should_translate() const;

    const std::vector<std::string>& get_airports() const;
    std::string get_airport_string() const;

    bool has_invalid_args() const;
    const std::vector<std::string>& get_invalid_args() const;
    std::string get_invalid_arg_string() const;

private:
    bool should_translate_;
    std::vector<std::string> airports_;
    std::vector<std::string> invalid_args_;

    static bool check_airport(const std::string& airport_string);
};

#endif
