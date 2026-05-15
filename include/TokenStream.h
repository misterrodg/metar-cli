#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include <string>
#include <vector>

struct TokenStream {
    std::vector<std::string> tokens;
    size_t pos = 0;

    bool eof() const;
    std::string_view peek(size_t lookahead = 0) const;
    std::string consume();

    TokenStream(std::vector<std::string> input);
};

#endif
