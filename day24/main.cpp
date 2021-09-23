#include <numeric>
#include "solution.h"

const bool debug = false;

class day24 : public aoc::solution {
protected:

    uint64_t qe_for_balanced_packages(const std::vector<uint64_t>& weights, uint64_t group_weight) {
        if (debug) {
            std::cout << "Need to find groups with weight of " << group_weight << std::endl;
        }
        uint64_t result_qe{0};

        bool result_found{false};
        for (size_t i = 1; (i < weights.size() - 2) && !result_found; i++) {
            auto comb = aoc::combinations(weights, i);
            do {
                auto items = comb.get();
                uint64_t this_total = std::accumulate(begin(items), end(items), 0ULL);
                uint64_t this_qe = std::accumulate(begin(items), end(items), 1ULL, std::multiplies<>());

                if (this_total == group_weight) {
                    if (debug) {
                        std::cout << "Group: ";
                        for (auto itm: items) {
                            std::cout << itm << ", ";
                        }
                        std::cout << std::endl;
                        std::cout << "This QE: " << this_qe << std::endl;
                        std::cout << "Result QE: " << result_qe << std::endl;
                    }
                    if (!result_found || result_qe > this_qe) {
                        result_qe = this_qe;
                        result_found = true;
                        if (debug) std::cout << "New QE found" << std::endl;
                    }
                }
            } while (comb.next());
        }
        return result_qe;
    }

    void run(std::istream& in, std::ostream& out) override {
        auto weights = aoc::elements_from_stream<uint64_t>(in);
        uint64_t total = std::accumulate(weights.begin(), weights.end(), 0ULL);
        if (debug) {
            std::cout << "Total = " << total << std::endl;
        }

        out << qe_for_balanced_packages(weights, total / 3) << std::endl;
        out << qe_for_balanced_packages(weights, total / 4) << std::endl;
    }
};

int main(int argc, char **argv) {
    day24 solution{};
    return solution.execute(argc, argv);
}
