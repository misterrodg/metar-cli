#include "TokenStream.h"
#include <stdexcept>

TokenStream::TokenStream(std::vector<std::string> input) : tokens(input) {
}

bool TokenStream::eof() const {
    return pos >= tokens.size();
}

std::string_view TokenStream::peek(size_t lookahead) const {
    size_t i = pos + lookahead;
    if (i >= tokens.size())
        return {};
    return tokens[i];
}

std::string TokenStream::consume() {
    if (eof())
        throw std::runtime_error("Unexpected end of token stream.");
    return tokens[pos++];
}
