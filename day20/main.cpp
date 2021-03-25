#include "solution.h"

class day20 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        static std::array<size_t, 1'000'001> house_to_presents;

        // part 1
        for (size_t i = 1; i <= 1'000'000; i++) {
            for (size_t j = i; j < 1'000'001; j += i) {
                house_to_presents[j] += i * 10;
            }
        }

        size_t part_1{0};
        for (size_t i = 0; i < 1'000'001; i++) {
            if (house_to_presents[i] >= 36'000'000) {
                part_1 = i;
                break;
            }
        }
        out << part_1 << std::endl;

        // part 2
        house_to_presents.fill(0);
        for (size_t i = 1; i <= 1'000'000; i++) {
            for (size_t j = i, times = 1; j < 1'000'000; j += i, times++) {
                if (times < 50) {
                    house_to_presents[j] += i * 11;
                } else {
                    break;
                }
            }
        }

        size_t part_2{0};
        for (size_t i = 0; i < 1'000'001; i++) {
            if (house_to_presents[i] >= 36'000'000) {
                part_2 = i;
                break;
            }
        }
        out << part_2 << std::endl;
    }
};

int main(int argc, char** argv) {
    day20 solution;
    return solution.execute(argc, argv);
}
