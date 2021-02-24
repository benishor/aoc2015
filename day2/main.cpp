#include "solution.h"
#include <regex>
#include <string>

class day2 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {

        auto part1{0L};
        auto part2{0L};
        long sides[3] = {0, 0, 0};
        for (std::string line; std::getline(in, line);) {
            std::string_view wstr = line;
            size_t start = 0;
            size_t end = 0;
            while (wstr.at(end) != 'x') end++;
            sides[0] = std::stol(std::string(wstr.substr(start, end).data(), end - start));
            start = ++end;
            while (wstr.at(end) != 'x') end++;
            sides[1] = std::stol(std::string(wstr.substr(start, end).data(), end - start));
            start = ++end;
            sides[2] = std::stol(std::string(wstr.substr(start).data()));

            std::sort(sides, sides + 3);
            auto area_1 = sides[0] * sides[1];
            auto area_2 = sides[0] * sides[2];
            auto area_3 = sides[1] * sides[2];

            auto area = 2 * area_1 + 2 * area_2 + 2 * area_3;
            auto smallest_side_area = area_1;
            part1 += area + smallest_side_area;

            part2 += 2 * sides[0] + 2 * sides[1] + smallest_side_area * sides[2];
        }
        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "2x3x4",
                .expected_output = "58\n34"
        });
    }
};

int main(int argc, char** argv) {
    day2 solution;
    return solution.execute(argc, argv);
}
