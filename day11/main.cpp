#include "solution.h"

static bool is_password_valid(const std::array<char, 8>& password) {
    char previous_char = '\0';
    std::set<char> pairs;
    char prev_difference_count{0};
    bool increasing_letters_found{false};
    for (auto& c : password) {
        if (c == 'i' || c == 'o' || c == 'l') return false;
        if (c == previous_char) {
            pairs.insert(c);
        }

        if (!increasing_letters_found) {
            if (previous_char + 1 == c) {
                prev_difference_count++;
            } else {
                prev_difference_count = 0;
            }
            if (prev_difference_count >= 2) {
                increasing_letters_found = true;
            }
        }
        previous_char = c;
    }
    if (!increasing_letters_found || pairs.size() < 2) {
        return false;
    }
    return true;
}

static void next_password(std::array<char, 8>& password) {
    for (size_t i = 7; i > 0; i--) {
        if (password[i] == 'z') {
            password[i] = 'a';
        } else {
            password[i]++;
            break;
        }
    }
}

class day11 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {
        std::string password_as_string = aoc::elements_from_stream<std::string>(in)[0];
        std::array<char, 8> password = {
                password_as_string.at(0),
                password_as_string.at(1),
                password_as_string.at(2),
                password_as_string.at(3),
                password_as_string.at(4),
                password_as_string.at(5),
                password_as_string.at(6),
                password_as_string.at(7)};
        do {
            next_password(password);
        } while (!is_password_valid(password));

        auto part1 = std::string(std::begin(password), std::end(password));

        do {
            next_password(password);
        } while (!is_password_valid(password));
        auto part2 = std::string(std::begin(password), std::end(password));

        out << part1 << std::endl;
        out << part2 << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "ghijklmn",
                .expected_output = "ghjaabcc\nghjbbcdd"
        });
    }
};

int main(int argc, char** argv) {
    day11 solution;
    return solution.execute(argc, argv);
}
