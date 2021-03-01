#include "solution.h"

class day13 : public aoc::solution {
protected:

    static long long get_current_satisfaction(const std::vector<std::string>& order,
                                              const std::map<std::string, std::map<std::string, long>>& preferences) {
        long long current_satisfaction{0};
        for (size_t person = 0; person < order.size(); person++) {
            size_t previous = person == 0 ? order.size() - 1 : person - 1;
            size_t next = person == order.size() - 1 ? 0 : person + 1;
            if (order[person] == "_me_") continue;
            current_satisfaction += order[previous] == "_me_" ? 0 : preferences.at(order[person]).at(order[previous]);
            current_satisfaction += order[next] == "_me_" ? 0 : preferences.at(order[person]).at(order[next]);
        }
        return current_satisfaction;
    }

    static long long get_max_satisfaction(const std::set<std::string>& persons,
                                          const std::map<std::string, std::map<std::string, long>>& preferences) {
        auto perms = aoc::permutations(persons);
        long long result = std::numeric_limits<long long>::min();
        do {
            auto order = perms.get();
            auto current_satisfaction = get_current_satisfaction(order, preferences);
            if (current_satisfaction > result) {
                result = current_satisfaction;
            }
        } while (perms.next());
        return result;
    }

    void run(std::istream& in, std::ostream& out) override {

        std::set<std::string> persons;
        std::map<std::string, std::map<std::string, long>> preferences;

        auto parse_re = std::regex(R"(^(.*) would (gain|lose) (\d+) .*to (.*)\.$)");
        for (std::string line; std::getline(in, line);) {
            std::smatch sm{};
            if (std::regex_match(line, sm, parse_re)) {
                auto a = sm.str(1);
                auto b = sm.str(4);
                auto amount = (sm.str(2) == "lose" ? -1 : 1) * std::stol(sm.str(3));

                persons.insert(a);
                persons.insert(b);
                preferences[a][b] = amount;
            }
        }

        auto part1 = get_max_satisfaction(persons, preferences);
        persons.insert("_me_");
        auto part2 = get_max_satisfaction(persons, preferences);

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "Alice would gain 54 happiness units by sitting next to Bob.\n"
                         "Alice would lose 79 happiness units by sitting next to Carol.\n"
                         "Alice would lose 2 happiness units by sitting next to David.\n"
                         "Bob would gain 83 happiness units by sitting next to Alice.\n"
                         "Bob would lose 7 happiness units by sitting next to Carol.\n"
                         "Bob would lose 63 happiness units by sitting next to David.\n"
                         "Carol would lose 62 happiness units by sitting next to Alice.\n"
                         "Carol would gain 60 happiness units by sitting next to Bob.\n"
                         "Carol would gain 55 happiness units by sitting next to David.\n"
                         "David would gain 46 happiness units by sitting next to Alice.\n"
                         "David would lose 7 happiness units by sitting next to Bob.\n"
                         "David would gain 41 happiness units by sitting next to Carol.",
                .expected_output = "330"
        });
    }
};


int main(int argc, char** argv) {
    day13 solution;
    return solution.execute(argc, argv);
}
