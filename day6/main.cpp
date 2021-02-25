#include "solution.h"

class day6 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        std::regex coords_re{R"(^([^\d]+)([\d]+),([\d]+)[^\d]+([\d]+),([\d]+)$)"};

        unsigned int lights_part1[1000 * 1000];
        memset(lights_part1, 0, sizeof(lights_part1));

        unsigned int lights_part2[1000 * 1000];
        memset(lights_part2, 0, sizeof(lights_part2));

        for (std::string line; std::getline(in, line);) {
            std::smatch sm{};
            if (std::regex_match(line, sm, coords_re)) {
                auto action = aoc::trim(sm.str(1));
                auto from_x = std::stoi(sm.str(2));
                auto from_y = std::stoi(sm.str(3));
                auto to_x = std::stoi(sm.str(4));
                auto to_y = std::stoi(sm.str(5));

                if (action == "turn on") {
                    for (auto y = from_y; y <= to_y; y++) {
                        size_t offset = y * 1000 + from_x;
                        for (auto x = from_x; x <= to_x; x++) {
                            lights_part1[offset] = 1;
                            lights_part2[offset]++;
                            offset++;
                        }
                    }
                } else if (action == "turn off") {
                    for (auto y = from_y; y <= to_y; y++) {
                        size_t offset = y * 1000 + from_x;
                        for (auto x = from_x; x <= to_x; x++) {
                            lights_part1[offset] = 0;
                            if (lights_part2[offset] > 0) lights_part2[offset]--;
                            offset++;
                        }
                    }
                } else {
                    for (auto y = from_y; y <= to_y; y++) {
                        size_t offset = y * 1000 + from_x;
                        for (auto x = from_x; x <= to_x; x++) {
                            lights_part1[offset] ^= 1;
                            lights_part2[offset] += 2;
                            offset++;
                        }
                    }
                }
            }
        }

        unsigned long part1 = 0;
        for (bool offset : lights_part1) {
            part1 += offset;
        }

        unsigned long part2 = 0;
        for (unsigned int offset : lights_part2) {
            part2 += offset;
        }

        out << part1 << std::endl;
        out << part2 << std::endl;
    }
};

int main(int argc, char** argv) {
    day6 solution;
    return solution.execute(argc, argv);
}
