#ifndef TOKEN_H
#define TOKEN_H

#include <algorithm>

enum token_t {
    VAR, ARRAY, OF, N, NT,
    START, LPAREN, RPAREN, SEMICOLON, COLON, DOUBLEDOT, COMMA, END
};

struct token {
    token_t tt;
    std::string str;

    token(token_t t, const std::string& str) :
            tt (t),
            str(std::move(str))
    {}
    token() :
            tt(START),
            str("")
    {}
};

#endif
