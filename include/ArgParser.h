#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>
#include <vector>

class ArgParser {
public:
    ArgParser(int argc, char* argv[]);
    bool get_use_translation() const;
    std::string get_airport_string() const;

private:
    bool m_use_translation;
    std::vector<std::string> m_airports;

    bool check_airport(const std::string& airport_string);
};

#endif
