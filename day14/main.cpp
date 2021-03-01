#include "solution.h"

struct reindeer_stats {
    std::string name;
    size_t speed{0};
    size_t active_seconds{0};
    size_t rest_seconds{0};
    size_t seconds_passed{0};
    size_t distance_travelled{0};
    size_t points{0};

    [[nodiscard]] size_t distance_after_seconds(size_t seconds) const {
        size_t cycle_seconds = active_seconds + rest_seconds;
        size_t result = (seconds / cycle_seconds) * active_seconds * speed;
        result += std::min(active_seconds, seconds % cycle_seconds) * speed;
        return result;
    }

    void tick_one_second() {
        size_t cycle_seconds = active_seconds + rest_seconds;
        if ((seconds_passed % cycle_seconds) < active_seconds) {
            distance_travelled += speed;
        }
        seconds_passed++;
    }

    void award_point() {
        points++;
    }

};

class day14 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        std::map<std::string, reindeer_stats> stats;

        auto stats_re = std::regex(R"(^(.*) can fly (\d+) km.*for (\d+) seconds.*rest for (\d+) seconds.$)");
        for (std::string line; std::getline(in, line);) {
            std::smatch sm{};
            if (std::regex_match(line, sm, stats_re)) {
                reindeer_stats s{
                        .name = sm.str(1),
                        .speed = static_cast<size_t>(std::stoi(sm.str(2))),
                        .active_seconds = static_cast<size_t>(std::stoi(sm.str(3))),
                        .rest_seconds = static_cast<size_t>(std::stoi(sm.str(4)))
                };
                stats[s.name] = s;
            }
        }

        size_t part1{0};
        for (auto& kv : stats) {
            auto d = kv.second.distance_after_seconds(2503);
            if (d > part1) {
                part1 = d;
            }
        }

        for (size_t i = 0; i < 2503; i++) {
            size_t leading_distance{0};
            for (auto& kv : stats) {
                kv.second.tick_one_second();
                if (kv.second.distance_travelled > leading_distance) {
                    leading_distance = kv.second.distance_travelled;
                }
            }

            for (auto& kv : stats) {
                if (kv.second.distance_travelled == leading_distance) {
                    kv.second.award_point();
                }
            }
        }

        size_t part2{0};
        for (auto& kv : stats) {
            if (kv.second.points > part2) {
                part2 = kv.second.points;
            }
        }

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.\n"
                         "Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.",
                .expected_output = "2660\n1564"
        });
    }
};

int main(int argc, char** argv) {
    day14 solution;
    return solution.execute(argc, argv);
}
