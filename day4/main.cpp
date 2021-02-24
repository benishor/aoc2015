#include "solution.h"
#include "md5.h"

class day4 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        std::string secret;
        std::getline(in, secret);

        auto part1{0UL}, part2{0UL};
        unsigned long key{1};
        std::stringstream hash_input;

        while (true) {
            hash_input.str("");
            hash_input << secret << key;

            auto md5 = MD5(hash_input.str());
            auto digest = md5.hexdigest();

            if (digest.substr(0, 5) == "00000") {
                if (part1 == 0UL) {
                    part1 = key;
                }
                if (digest.at(5) == '0') {
                    part2 = key;
                    break;
                }
            }
            key++;
        }

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "abcdef",
                .expected_output = "609043\n6742839"
        });
    }
};

int main(int argc, char** argv) {
    day4 solution{};
    return solution.execute(argc, argv);
}
