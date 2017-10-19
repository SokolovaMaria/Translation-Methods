#include "parser.h"

parser::graph_iterator parser::parse(std::string const &input) {
    lex = std::make_shared<lex_analyser>(std::move(input));
    lex->next_token();
    return graph_iterator(s());
}

void parser::skip(token_t const &type, node* node) {
    if (lex->get_cur_token().tt == type) {
        node->add_child(lex->get_cur_token().str);
        lex->next_token();
    } else {
        throw std::runtime_error("unexpected symbol at pos: " + std::to_string(lex->get_cur_pos()));
    }
}

std::shared_ptr<parser::node> parser::s() {
    node res("S");
    switch (lex->get_cur_token().tt) {
        case VAR :
            res.add_child(lex->get_cur_token().str);
            lex->next_token();
            res.add_child(arr());
            break;
        default:
            throw std::runtime_error("unexpected symbol at pos : " + std::to_string(lex->get_cur_pos()));
    }
    return std::make_shared<node>(res);
}

std::shared_ptr<parser::node> parser::arr() {
    node res("ARR");
    switch(lex->get_cur_token().tt) {
        case NT :
            res.add_child(names());
            skip(COLON, &res);
            res.add_child(arr_type());
            skip(SEMICOLON, &res);
            res.add_child(arr());
            break;
        case END:
            break;
        default:
            throw std::runtime_error("unexpected symbol at pos: " + std::to_string(lex->get_cur_pos()));
    }
    return std::make_shared<node>(res);
}

std::shared_ptr<parser::node> parser::names() {
    node res("NAMES");
    switch (lex->get_cur_token().tt) {
        case NT :
            res.add_child(lex->get_cur_token().str);
            lex->next_token();
            res.add_child(names_tail());
            break;
        default:
            throw std::runtime_error("unexpected symbol at pos: " + std::to_string(lex->get_cur_pos()));
    }
    return std::make_shared<node>(res);
}

std::shared_ptr<parser::node> parser::names_tail() {
    node res("NAMES'");
    switch(lex->get_cur_token().tt) {
        case COMMA :
            res.add_child(lex->get_cur_token().str);
            lex->next_token();
            res.add_child(names());
            break;
        default:
            break;
    }
    return std::make_shared<node>(res);
}

std::shared_ptr<parser::node> parser::arr_type() {
    node res("ARRTYPE");
    switch(lex->get_cur_token().tt) {
        case ARRAY :
            res.add_child(lex->get_cur_token().str);
            lex->next_token();
            skip(LPAREN, &res);
            res.add_child(dim());
            skip(RPAREN, &res);
            skip(OF, &res);
            res.add_child(arr_type_tail());
            break;
        default:
            throw std::runtime_error("unexpected symbol at pos: " + std::to_string(lex->get_cur_pos()));
    }
    return std::make_shared<node>(res);
}

std::shared_ptr<parser::node> parser::arr_type_tail() {
    node res("ARRTYPE'");
    switch (lex->get_cur_token().tt) {
        case ARRAY :
            res.add_child(arr_type());
            break;
        case NT:
            res.add_child(lex->get_cur_token().str);
            lex->next_token();
            break;
        default:
            throw std::runtime_error("unexpected symbol at pos: " + std::to_string(lex->get_cur_pos()));
    }
    return std::make_shared<node>(res);
}

std::shared_ptr<parser::node> parser::dim() {
    node res("DIM");
    switch(lex->get_cur_token().tt) {
        case N :
            res.add_child(lex->get_cur_token().str);
            lex->next_token();
            skip(DOUBLEDOT, &res);
            skip(N, &res);
            res.add_child(dim_tail());
            break;
        default:
            throw std::runtime_error("unexpected symbol at pos: " + std::to_string(lex->get_cur_pos()));
    }
    return std::make_shared<node>(res);
}

std::shared_ptr<parser::node> parser::dim_tail() {
    node res("DIM'");
    switch (lex->get_cur_token().tt) {
        case COMMA :
            res.add_child(lex->get_cur_token().str);
            lex->next_token();
            res.add_child(dim());
            break;
        default:
            break;
    }
    return std::make_shared<node>(res);
}

std::vector<parser::graph_iterator> parser::graph_iterator::get_children() {
    std::vector<graph_iterator> children;
    for (auto child : root->children) {
        children.push_back(graph_iterator(child));
    }
    return children;
}

std::string parser::graph_iterator::get_str() {
    return root->str;
}
