#include "solution.h"
#include <unordered_set>
#include <stdexcept>

enum class direction {
    Up,
    Down,
    Left,
    Right
};

direction direction_from_symbol(char c) {
    switch (c) {
        case '^':
            return direction::Up;
        case 'v':
            return direction::Down;
        case '<':
            return direction::Left;
        case '>':
            return direction::Right;
        default:
            throw std::runtime_error("Invalid direction " + c);
    }
}

struct position {
    int x{0}, y{0};

    void go(direction d) {
        switch (d) {
            case direction::Up:
                y--;
                break;
            case direction::Down:
                y++;
                break;
            case direction::Left:
                x--;
                break;
            case direction::Right:
                x++;
                break;
        }
    }
};

// set needs <, unordered_set needs hash and equals
bool operator==(const position& a, const position& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator<(const position& a, const position& b) {
    return a.y == b.y ? a.x < b.y : (a.y < b.y);
}

class position_hash {
public:
    std::size_t operator()(const position& p) const noexcept {
        return (uint64_t) p.y << 32 | (uint64_t) p.x;
    }
};

class day3 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        std::unordered_set<position, position_hash> visited_houses_part1;
        position santa_part1{0, 0};
        visited_houses_part1.insert(santa_part1);

        std::unordered_set<position, position_hash> visited_houses_part2;
        position santa_part2{0, 0};
        position robo_part2{0, 0};
        visited_houses_part2.insert({santa_part2, robo_part2});

        bool is_robo_turn = false;
        for (char c; in >> c; is_robo_turn = !is_robo_turn) {
            auto dir = direction_from_symbol(c);
            santa_part1.go(dir);
            visited_houses_part1.insert(santa_part1);

            if (is_robo_turn) {
                robo_part2.go(dir);
                visited_houses_part2.insert(robo_part2);
            } else {
                santa_part2.go(dir);
                visited_houses_part2.insert(santa_part2);
            }
        }
        out << visited_houses_part1.size() << std::endl;
        out << visited_houses_part2.size() << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "^>v<",
                .expected_output = "4\n3"
        });
    }
};

int main(int argc, char** argv) {
    day3 solution;
    return solution.execute(argc, argv);
}
