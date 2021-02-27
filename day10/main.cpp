#include "solution.h"

class day10 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        std::string input;
        std::getline(in, input);

        size_t part1{0}, part2{0};
        for (size_t times = 0; times < 50; times++) {
            std::stringstream output;

            char previous_char{'\0'};
            size_t char_counter{0};
            for (auto c : input) {
                if (c == previous_char) {
                    char_counter++;
                } else {
                    if (char_counter > 0) {
                        output << char_counter << previous_char;
                    }
                    previous_char = c;
                    char_counter = 1;
                }
            }
            output << char_counter << previous_char;
            input = output.str();
            if (times == 39) part1 = input.size();
            if (times == 49) part2 = input.size();
        }

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "1",
                .expected_output = "82350\n1166642"
        });
    }
};


int main(int argc, char** argv) {
    day10 solution;
    return solution.execute(argc, argv);
}
