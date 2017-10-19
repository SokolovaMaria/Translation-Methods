#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "lex_analyser.h"

struct parser {

    struct graph_iterator;
    graph_iterator parse(const std::string& input);

private:
    struct node {
        std::string str;
        std::vector<std::shared_ptr<node> > children;

        node(const std::string& str) :
                str(std::move(str)) {}

        void add_child(std::shared_ptr<node> n) {
            children.push_back(n);
        }

        void add_child(const std::string& str) {
            children.push_back(std::make_shared<node>(str));
        }
    };

    std::shared_ptr<lex_analyser> lex;

    void skip(token_t const &type, node *node);

    std::shared_ptr<node> s();
    std::shared_ptr<node> arr();
    std::shared_ptr<node> names();
    std::shared_ptr<node> names_tail();
    std::shared_ptr<node> arr_type();
    std::shared_ptr<node> arr_type_tail();
    std::shared_ptr<node> dim();
    std::shared_ptr<node> dim_tail();
};

struct parser::graph_iterator {

    friend graph_iterator parser::parse(const std::string& str);
    std::vector<graph_iterator> get_children();
    std::string get_str();

private:
    std::shared_ptr<node> root;
    graph_iterator(std::shared_ptr<node>&& s) :
            root(std::move(s))
    {}
    graph_iterator(std::shared_ptr<node>& s) :
            root(s)
    {}
};

#endif
