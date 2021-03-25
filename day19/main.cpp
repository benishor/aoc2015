#include "solution.h"

using rule = std::pair<std::string, std::string>;

class day19 : public aoc::solution {
protected:

    bool find_minimum_path(const std::string& from,
                           const std::vector<rule>& replacements,
                           size_t depth,
                           size_t& min_depth) {

        static std::string target = "e";
        if (from == target) {
            if (depth < min_depth) {
                min_depth = depth;
            }
            return true;
        }

        if (depth >= min_depth) return false;

        std::vector<std::string> replacement_state;
        for (auto& r : replacements) {
            auto outputs = perform_replace_backwards(from, r);
            for (auto& o : outputs) {
                if (o != from) {
                    replacement_state.push_back(o);
                }
            }
        }

        for (auto& s : replacement_state) {
            if (find_minimum_path(s, replacements, depth + 1, min_depth))
                return true;
        }
        return false;
    }

    void run(std::istream& in, std::ostream& out) override {

        bool reading_start_string{false};
        std::string start_string;
        std::vector<rule> replacements;
        for (std::string line; std::getline(in, line);) {
            if (reading_start_string) {
                start_string = aoc::trim(line);
            } else {
                if (aoc::trim(line).empty()) {
                    reading_start_string = true;
                } else {
                    auto pieces = aoc::split_string(line, " => ");
                    replacements.emplace_back(pieces[0], pieces[1]);
                }
            }
        }

        // part 1
        std::set<std::string> unique_molecules;
        for (auto& r : replacements) {
            auto outputs = perform_replace(start_string, r);
            unique_molecules.insert(std::begin(outputs), std::end(outputs));
        }

        out << unique_molecules.size() << std::endl;

        // part 2
        std::string start = "e";
        std::string target = start_string;
        size_t min_depth = std::numeric_limits<size_t>::max();


        std::sort(std::begin(replacements), std::end(replacements), [](const rule& a, const rule& b) {
            return a.second.size() > b.second.size();
        });

        find_minimum_path(target, replacements, 0, min_depth);
        out << min_depth << std::endl;
    }

    static std::vector<std::string> perform_replace(const std::string& input, const rule& r) {
        std::vector<std::string> result;

        size_t pos = 0;
        while ((pos = input.find(r.first, pos)) != std::string::npos) {
            result.push_back(input.substr(0, pos) + r.second + input.substr(pos + r.first.size()));
            pos += r.first.size();
        }
        return result;
    }

    static std::vector<std::string> perform_replace_backwards(const std::string& input, const rule& r) {
        std::vector<std::string> result;

        size_t pos = 0;
        while ((pos = input.find(r.second, pos)) != std::string::npos) {
            result.push_back(input.substr(0, pos) + r.first + input.substr(pos + r.second.size()));
            pos += r.second.size();
        }
        return result;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "e => H\n"
                         "e => O\n"
                         "H => HO\n"
                         "H => OH\n"
                         "O => HH\n"
                         "\nHOHOHO",
                .expected_output = "7\n6"
        });
    }
};

int main(int argc, char** argv) {
    day19 solution;
    return solution.execute(argc, argv);
}
