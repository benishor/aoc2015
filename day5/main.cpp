#include "solution.h"

bool is_string_nice_1(const std::string& s) {
    char previous_char = '\0';
    static std::map<char, char> vowels = {
            {'a', 0},
            {'e', 1},
            {'i', 2},
            {'o', 3},
            {'u', 4},
    };

    size_t string_vowels{0};
    bool double_letter_found{false};

    for (auto c : s) {
        if (vowels.count(c)) {
            string_vowels++;
        }
        if (c == previous_char) {
            double_letter_found = true;
        }
        if (c == 'b' || c == 'd' || c == 'q' || c == 'y') {
            if (previous_char == c - 1) {
                return false;
            }
        }
        previous_char = c;
    }

    if (!double_letter_found) {
        return false;
    }
    if (string_vowels < 3) {
        return false;
    }
    return true;
}

bool is_string_nice_2(const std::string& s) {
    char previous_char = '\0';
    std::map<int, std::vector<size_t>> sets;
    bool has_letter_between = false;
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (previous_char != '\0') {
            sets[previous_char << 8 | c].push_back(i - 1);
        }
        if (i < s.length() - 1 && previous_char == s[i + 1]) {
            has_letter_between = true;
        }
        previous_char = c;
    }
    if (!has_letter_between) return false;

    for (auto& kv : sets) {
        if (kv.second.size() > 1) {
            for (size_t i = 0; i < kv.second.size() - 1; i++) {
                for (size_t j = i; j < kv.second.size(); j++) {
                    if (kv.second[j] - kv.second[i] > 1) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

class day5 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {
        size_t part1{0L};
        size_t part2{0L};
        for (std::string line; std::getline(in, line);) {
            part1 += is_string_nice_1(line);
            part2 += is_string_nice_2(line);
        }
        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "qjhvhtzxzqqjkmpb\naaa\njchzalrnumimnmhp\nhaegwjzuvuyypxyu\ndvszwmarrgswjxmb",
                .expected_output = "1\n1"
        });
    }
};

int main(int argc, char** argv) {
    day5 solution;
    return solution.execute(argc, argv);
}
