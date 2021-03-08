#include <numeric>
#include "solution.h"

struct ingredient {
    std::string name;
    std::map<std::string, long long> attributes;
};

class day15 : public aoc::solution {
protected:

    static ingredient parse_ingredient(const std::string& line) {
        auto pieces = aoc::split_string(line, ":");
        if (pieces.size() != 2) {
            throw std::runtime_error("Bad ingredient line [" + line + "]");
        }

        auto new_ingredient = ingredient{};
        new_ingredient.name = aoc::trim(pieces[0]);
        std::smatch sm{};

        auto properties_re = std::regex{R"(([^\s]+) ([-\d]+)(,|$))"};
        std::string::const_iterator search_start(pieces[1].cbegin());
        while (std::regex_search(search_start, pieces[1].cend(), sm, properties_re)) {
            auto attribute = aoc::trim(sm.str(1));
            auto value = std::stol(aoc::trim(sm.str(2)));
            new_ingredient.attributes[attribute] = value;
            search_start = sm.suffix().first;
        }

        return new_ingredient;
    }

    static std::tuple<size_t, size_t>
    score_for_combination(const std::map<std::string, ingredient>& ingredients, std::vector<int> spoons) {

        auto new_ingredient = ingredient{};

        auto kv = ingredients.begin();
        for (size_t i = 0; i < ingredients.size(); i++) {
            for (auto& kv2 : kv->second.attributes) {
                auto attr = kv2.first;
                auto value = kv2.second;
                new_ingredient.attributes[attr] += spoons[i] * value;
            }
            kv++;
        }

        size_t total{1};
        for (auto& kv : new_ingredient.attributes) {
            auto attr = kv.first;
            auto value = kv.second;
            if (attr == "calories") continue;
            if (value <= 0) {
                return {0, 0};
            } else {
                total *= value;
            }
        }

        return {total, new_ingredient.attributes["calories"] <= 0 ? 0 : new_ingredient.attributes["calories"]};
    }

    static bool is_spoon_set_valid(const std::vector<int>& set) {
        return std::accumulate(std::begin(set), std::end(set), 0L) == 100;
    }

    void run(std::istream& in, std::ostream& out) override {

        std::map<std::string, ingredient> ingredients;
        for (std::string line; std::getline(in, line);) {
            auto new_ingredient = parse_ingredient(line);
            ingredients[new_ingredient.name] = new_ingredient;
        }

        std::vector<int> spoons(101);
        std::iota(std::begin(spoons), std::end(spoons), 0);

        auto combs = aoc::combinations(spoons, ingredients.size());
        size_t part1{0}, part2{0};
        do {
            std::vector<int> current_spoons = combs.get();
            if (is_spoon_set_valid(current_spoons)) {
                auto perms = aoc::permutations(current_spoons);
                do {
                    std::vector<int> sp = perms.get();
                    auto scores = score_for_combination(ingredients, sp);
                    auto score = std::get<0>(scores);
                    auto calories = std::get<1>(scores);
                    if (score > part1) {
                        part1 = score;
                    }
                    if (calories == 500 && score > part2) {
                        part2 = score;
                    }
                } while (perms.next());
            }
        } while (combs.next());

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8\n"
                         "Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3",
                .expected_output = "62842880"
        });
    }
};


int main(int argc, char** argv) {
    day15 solution;
    return solution.execute(argc, argv);
}
