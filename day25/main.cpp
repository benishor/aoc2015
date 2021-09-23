#include "solution.h"

class day25 : public aoc::solution {
protected:

    static uint64_t code_for_row_and_col(uint64_t desired_row, uint64_t desired_col) {
        uint64_t row{1};
        uint64_t col{1};
        uint64_t depth{1};
        uint64_t code{20151125};

        while (row < desired_row || col < desired_col) {
            if (row > 1) {
                col++;
                row--;
            } else {
                depth++;
                row = depth;
                col = 1;
            }
            code = (code * 252533ULL) % 33554393ULL;
        }

        return code;
    }

    void run(std::istream& in, std::ostream& out) override {
        out << code_for_row_and_col(2947, 3029) << std::endl;
    }
};

int main(int argc, char **argv) {
    day25 solution{};
    return solution.execute(argc, argv);
}
