#include <numeric>
#include "solution.h"

class day17 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        auto volumes = aoc::elements_from_stream<int>(in);
        std::sort(volumes.rbegin(), volumes.rend());

        std::map<int, int> stats{};
        for (size_t i = 0; i < volumes.size(); i++) {
            count_possibilities(volumes, i, 150, stats);
        }

        size_t part1 = std::accumulate(std::begin(stats), std::end(stats), 0,
                                       [](int value, const auto& p) {
                                           return value + p.second;
                                       }
        );
        int part2 = stats.begin()->second;

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void count_possibilities(std::vector<int>& volumes, size_t index, int remaining, std::map<int, int>& stats,
                             int depth = 0) {
        if (volumes[index] == remaining) {
            stats[depth]++;
            return;
        }
        if (volumes[index] > remaining) return;
        for (size_t i = index + 1; i < volumes.size(); i++) {
            count_possibilities(volumes, i, remaining - volumes[index], stats, depth + 1);
        }
    }
};

int main(int argc, char** argv) {
    day17 solution;
    return solution.execute(argc, argv);
}
