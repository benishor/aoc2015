#include "solution.h"

class day9 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        std::map<std::string, size_t> cities;
        std::map<unsigned int, size_t> distances;

        std::regex distance_re{R"(^(.*) to (.*) = (\d+)$)"};
        size_t city_id = 0;
        for (std::string line; std::getline(in, line);) {
            std::smatch sm{};
            if (std::regex_match(line, sm, distance_re)) {
                auto from = aoc::trim(sm.str(1));
                auto to = aoc::trim(sm.str(2));
                auto distance = std::stol(aoc::trim(sm.str(3)));

                size_t from_city_id;
                if (cities.count(from)) {
                    from_city_id = cities[from];
                } else {
                    from_city_id = city_id;
                    cities[from] = city_id++;
                }

                size_t to_city_id;
                if (cities.count(to)) {
                    to_city_id = cities[to];
                } else {
                    to_city_id = city_id;
                    cities[to] = city_id++;
                }

                distances[from_city_id << 8 | to_city_id] = distance;
                distances[to_city_id << 8 | from_city_id] = distance;
            }
        }

        std::vector<size_t> city_ids;
        city_ids.reserve(cities.size());
        for (auto& kv : cities) {
            city_ids.push_back(kv.second);
        }


        size_t lowest_distance = std::numeric_limits<size_t>::max();
        size_t largest_distance = std::numeric_limits<size_t>::min();

        auto perms = aoc::permutations(city_ids);
        while (perms.next()) {
            size_t current_distance{0};
            auto order = perms.get();
            size_t from_city_id = *order.begin();
            for (size_t i = 1; i < order.size(); i++) {
                size_t to_city_id = order[i];
                current_distance += distances[from_city_id << 8 | to_city_id];
                from_city_id = to_city_id;
            }
            if (current_distance < lowest_distance) {
                lowest_distance = current_distance;
            }
            if (current_distance > largest_distance) {
                largest_distance = current_distance;
            }
        }

        out << lowest_distance << std::endl;
        out << largest_distance << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "London to Dublin = 464\n"
                         "London to Belfast = 518\n"
                         "Dublin to Belfast = 141",
                .expected_output = "605\n982"
        });
    }
};

int main(int argc, char** argv) {
    day9 solution;
    return solution.execute(argc, argv);
}
