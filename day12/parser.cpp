#include "parser.h"

json_parser::json_parser(std::string source)
        : lex{std::move(source), this} {}

void json_parser::parse() {
    root = new json_node;
    parents.push(root);
    lex.run();
}

void json_parser::on_lexme(lexme* l) {
    switch (l->type) {
        case lexme_type::start_object: {
            parents.top()->type = json_node_type::object;
            reading_key = true;
            break;
        }
        case lexme_type::end_object:
            parents.pop();
            break;
        case lexme_type::start_array: {
            parents.top()->type = json_node_type::array;
            auto node = new json_node;
            parents.top()->children.push_back(node);
            parents.push(node);
            break;
        }
        case lexme_type::end_array:
            parents.pop();
            break;
        case lexme_type::comma:
            if (parents.top()->type == json_node_type::object) {
                reading_key = true;
            } else if (parents.top()->type == json_node_type::array) {
                auto node = new json_node;
                parents.top()->children.push_back(node);
                parents.push(node);
            }
            break;
        case lexme_type::colon:
//                reading_key = false;
            break;
        case lexme_type::string: {
            if (parents.top()->type == json_node_type::object && reading_key) {
                auto node = new json_node;
                parents.top()->attributes[l->value] = node;
                parents.push(node);
                reading_key = false;
            } else {
                parents.top()->type = json_node_type::string;
                parents.top()->value = l->value;
                parents.pop();
            }
            break;
        }
        case lexme_type::number: {
            parents.top()->type = json_node_type::number;
            parents.top()->value = l->value;
            parents.pop();
            break;
        }
    }
}
