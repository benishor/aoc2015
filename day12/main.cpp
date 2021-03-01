#include "solution.h"
#include "parser.h"

void wipe_out_objects_with_red(json_node** node);
void add_all_numbers(json_node* node, long long& result);

class day12 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        long long part1{0}, part2{0};
        for (std::string line; std::getline(in, line);) {
            json_parser parser{line};
            parser.parse();

            add_all_numbers(parser.root, part1);
            wipe_out_objects_with_red(&parser.root);
            add_all_numbers(parser.root, part2);
        }

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = R"([1,{"c":"red","b":2},3])",
                .expected_output = "6\n4"
        });
    }
};

void wipe_out_objects_with_red(json_node** node) {
    if (*node == nullptr) return;
    if ((*node)->type == json_node_type::array) {
        for (auto& c : (*node)->children) {
            wipe_out_objects_with_red(&c);
        }
    } else if ((*node)->type == json_node_type::object) {
        for (auto& kv : (*node)->attributes) {
            if (kv.second->type == json_node_type::string && kv.second->value == "red") {
                *node = nullptr;
                return;
            }
        }

        for (auto& kv : (*node)->attributes) {
            wipe_out_objects_with_red(&kv.second);
        }
    }
}

void add_all_numbers(json_node* node, long long& result) {
    if (node == nullptr) return;
    switch (node->type) {
        case json_node_type::number:
            result += std::stol(node->value);
            break;
        case json_node_type::array:
            for (auto& i : node->children) {
                add_all_numbers(i, result);
            }
            break;
        case json_node_type::object:
            for (auto& kv : node->attributes) {
                add_all_numbers(kv.second, result);
            }
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    day12 solution;
    return solution.execute(argc, argv);
}
