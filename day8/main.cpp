#include "solution.h"

class day8 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {

        size_t part1_code{0}, part1_mem{0}, part2_code{0};
        for (std::string line; std::getline(in, line);) {
            auto output = line.substr(1, line.size() - 2);

            size_t decoded_length{0};
            bool escaped{false};
            for (size_t i = 0; i < output.size();) {
                char c = output[i];
                if (escaped) {
                    if (c == '\\' || c == '"') {
                        i++;
                    } else if (c == 'x') {
                        i += 3;
                    }
                    escaped = false;

                } else {
                    if (c == '\\') {
                        escaped = true;
                    }
                    decoded_length++;
                    i++;
                }
            }

            auto line_code_size = line.size();
            auto line_mem_size = decoded_length;
            part1_code += line_code_size;
            part1_mem += line_mem_size;

            size_t encoded_length = 2 + line.size()
                                    + std::count(line.begin(), line.end(), '"')
                                    + std::count(line.begin(), line.end(), '\\');
            part2_code += encoded_length;

//            std::cout << line << " -> " << std::endl;
//            std::cout << "\tcode: " << line_code_size << std::endl;
//            std::cout << "\tmem: " << line_mem_size << std::endl;
//            std::cout << "\tencoded: " << encoded_length << std::endl;
        }
        out << (part1_code - part1_mem) << std::endl;
        out << (part2_code - part1_code) << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "\"\"\n\"abc\"\n\"aaa\\\"aaa\"\n\"\\x27\"",
                .expected_output = "12\n19"
        });
    }
};

int main(int argc, char** argv) {
    day8 solution;
    return solution.execute(argc, argv);
}
