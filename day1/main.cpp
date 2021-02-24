#include "solution.h"

class day1 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        int floor{0};
        int position{1};
        int basement_position{0};
        for (char c; in >> c; position++) {
            switch (c) {
                case '(':
                    floor++;
                    break;
                case ')':
                    floor--;
                    break;
                default:
                    break;
            }
            if (floor == -1 && basement_position == 0) {
                basement_position = position;
            }
        }
        out << floor << std::endl;
        out << basement_position << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = ")",
                .expected_output = "-1\n1"
        });
        add_test(aoc::test{
                .name = "Second",
                .input = ")())())",
                .expected_output = "-3\n5"
        });
    }

};

int main(int argc, char** argv) {
    day1 solution{};
    return solution.execute(argc, argv);
}
