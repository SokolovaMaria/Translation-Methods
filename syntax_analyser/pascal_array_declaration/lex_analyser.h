#ifndef SYNTAX_ANALYSER_LEXICAL_ANALYSER_H
#define SYNTAX_ANALYSER_LEXICAL_ANALYSER_H

#include <fstream>
#include "token.h"

struct lex_analyser {

    lex_analyser(const std::string str);

    void skip_whitespaces();

    void next_token();

    token get_cur_token() const;

    void next_char();

    int get_cur_pos();

    ~lex_analyser();

private:
    std::ifstream input_stream;
    size_t cur_pos;
    int cur_char;
    token cur_token;
};

#endif //SYNTAX_ANALYSER_LEXICAL_ANALYSER_H
