#include <iostream>
#include "lex_analyser.h"

lex_analyser::lex_analyser(const std::string input) {
    input_stream.open(input);
    cur_pos = 0;
    cur_token = token();
    cur_char = input_stream.get();
}

lex_analyser::~lex_analyser() {
    input_stream.close();
}

void lex_analyser::next_char() {
    cur_pos++;
    cur_char = input_stream.get();
}

int lex_analyser::get_cur_pos() {
    return cur_pos;
}

token lex_analyser::get_cur_token() const {
    return cur_token;
}

void lex_analyser::skip_whitespaces() {
    while(isspace(cur_char)) {
        next_char();
    }
}

void lex_analyser::next_token() {
    skip_whitespaces();
    std::string token;
    bool isname = false;
    while(isalpha(cur_char) || isdigit(cur_char)) {
        if (isalpha(cur_char)) isname = true;
        token += (cur_char);
        next_char();
    }
    if (!isname && !token.empty()) {
        cur_token = {N, token};
        return;
    }
    if (token == "var") {
        cur_token = {VAR, token};
        return;
    }
    if (token == "array") {
        cur_token = {ARRAY, token};
        return;
    }
    if (token == "of") {
        cur_token = {OF, token};
        return;
    }
    if (!token.empty()) {
        cur_token = {NT, token};
        return;
    }
    switch (cur_char) {
        case ':' :
            cur_token = {COLON, ":"};
            next_char();
            break;
        case ';':
            cur_token = {SEMICOLON, ";"};
            next_char();
            break;
        case '.':
            next_char();
            if (cur_char == '.') {
                cur_token = {DOUBLEDOT, ".."};
                next_char();
                break;
            } else {
                std::runtime_error("unexpected symbol at: " + std::to_string(cur_pos));
            }
        case ',':
            cur_token = {COMMA, ","};
            next_char();
            break;
        case '[':
            cur_token = {LPAREN, "["};
            next_char();
            break;
        case ']':
            cur_token = {RPAREN, "]"};
            next_char();
            break;
        case -1:
            cur_token = {END, ""};
            break;
        default:
            std::runtime_error("unexpected symbol at: " + std::to_string(cur_pos));

    }
}


