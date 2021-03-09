#include "solution.h"

class day16 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        std::map<std::string, int> needed_traits = {
                {"children",    3},
                {"cats",        7},
                {"samoyeds",    2},
                {"pomeranians", 3},
                {"akitas",      0},
                {"vizslas",     0},
                {"goldfish",    5},
                {"trees",       3},
                {"cars",        2},
                {"perfumes",    1},
        };

        int part1{0}, part2{0};

        auto sue_re = std::regex{R"(^Sue (\d+):(.*)$)"};
        std::smatch sm{};
        for (std::string line; std::getline(in, line);) {
            if (std::regex_match(line, sm, sue_re)) {
                auto aunt_number = std::stoi(sm.str(1));
                auto aunt_traits = parse_traits(sm.str(2));
                if (is_aunt_matching_part1(aunt_traits, needed_traits)) {
                    part1 = aunt_number;
                }
                if (is_aunt_matching_part2(aunt_traits, needed_traits)) {
                    part2 = aunt_number;
                }
            }
        }

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    static bool is_aunt_matching_part1(const std::map<std::string, int>& aunt_traits,
                                       const std::map<std::string, int>& needed_traits) {
        return std::all_of(needed_traits.cbegin(), needed_traits.cend(), [&](auto kv) {
            if (aunt_traits.count(kv.first) && aunt_traits.at(kv.first) != kv.second) {
                return false;
            } else {
                return true;
            }
        });
    }

    static bool is_aunt_matching_part2(const std::map<std::string, int>& aunt_traits,
                                       const std::map<std::string, int>& needed_traits) {
        return std::all_of(needed_traits.cbegin(), needed_traits.cend(), [&](auto kv) {
            if (aunt_traits.count(kv.first)) {
                if (kv.first == "cats" || kv.first == "trees") {
                    if (aunt_traits.at(kv.first) <= kv.second) {
                        return false;
                    }
                } else if (kv.first == "pomeranians" || kv.first == "goldfish") {
                    if (aunt_traits.at(kv.first) >= kv.second) {
                        return false;
                    }
                } else if (aunt_traits.at(kv.first) != kv.second) {
                    return false;
                }
            }
            return true;
        });
    }

    static std::map<std::string, int> parse_traits(const std::string& traits_line) {
        std::map<std::string, int> traits;
        std::smatch sm{};
        auto traits_re = std::regex{R"(\s*([^:]+): ([-\d]+)(,|$))"};
        std::string::const_iterator search_start(traits_line.cbegin());
        while (std::regex_search(search_start, traits_line.cend(), sm, traits_re)) {
            auto attribute = aoc::trim(sm.str(1));
            auto value = std::__cxx11::stoi(aoc::trim(sm.str(2)));
            traits[attribute] = value;
            search_start = sm.suffix().first;
        }
        return traits;
    }
};

int main(int argc, char** argv) {
    day16 solution{};
    return solution.execute(argc, argv);
}
