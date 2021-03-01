#pragma once

#include "lexer.h"
#include <vector>
#include <map>

enum class json_node_type {
    string,
    number,
    array,
    object,
    null
};

struct json_node {
    json_node_type type{json_node_type::null};
    std::vector<json_node*> children;
    std::map<std::string, json_node*> attributes;
    std::string value;
};

struct json_parser : lexer_listener {
    lexer lex;
    json_node* root{nullptr};
    std::stack<json_node*> parents;
    bool reading_key{false};

    explicit json_parser(std::string source);
    void parse();
    void on_lexme(lexme* l) override;
};
