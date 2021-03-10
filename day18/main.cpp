#include <numeric>
#include "solution.h"

const int grid_width = 100;
const int grid_height = 100;
using lights_state = std::bitset<grid_width * grid_height>;

class day18 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        lights_state original_state = read_state_from_stream(in);

        lights_state state1 = original_state;
        lights_state state2 = original_state;
        set_corners_on(state2);

        for (int iterations = 0; iterations < 100; iterations++) {
            state1 = next_state(state1);
            state2 = next_state(state2);
            set_corners_on(state2);
        }
        out << count_turned_on_lights(state1) << std::endl;
        out << count_turned_on_lights(state2) << std::endl;
    }

    static lights_state read_state_from_stream(std::istream& in) {
        lights_state state;
        int y = 0;
        for (std::string line; std::getline(in, line);) {
            int x = 0;
            for (auto c : line) {
                state.set(y * grid_width + x, c == '#');
                x++;
            }
            y++;
        }
        return state;
    }

    static void set_corners_on(lights_state& state) {
        state.set(0, true);
        state.set(grid_width - 1, true);
        state.set(grid_width * grid_height - 1, true);
        state.set(grid_width * (grid_height - 1), true);
    }

    static lights_state next_state(const lights_state& current) {
        lights_state result = current;
        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                auto offset = y * grid_width + x;
                auto neighbours = count_neighbours(current, x, y);
                if (current.test(offset)) {
                    if (neighbours < 2 || neighbours > 3) {
                        result.set(offset, false);
                    }
                } else {
                    if (neighbours == 3) {
                        result.set(offset, true);
                    }
                }
            }
        }

        return result;
    }

    static int count_neighbours(const lights_state& state, int x, int y) {
        int total{0};
        for (int addy = -1; addy <= 1; addy++) {
            for (int addx = -1; addx <= 1; addx++) {
                if (addy == 0 && addx == 0) continue;
                auto real_x = addx + x;
                auto real_y = addy + y;
                if (real_x >= 0 && real_x < grid_width && real_y >= 0 && real_y < grid_width) {
                    total += state.test(real_y * grid_width + real_x);
                }
            }
        }
        return total;
    }

    static size_t count_turned_on_lights(const lights_state& state) {
        return state.count();
    }

    static void dump_state(const lights_state& state) {
        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                std::cout << (state[y * grid_width + x] ? '#' : '.');
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

int main(int argc, char** argv) {
    day18 solution;
    return solution.execute(argc, argv);
}
