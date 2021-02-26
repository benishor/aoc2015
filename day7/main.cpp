#include <utility>

#include "solution.h"

enum class source_type {
    Value,
    And,
    Or,
    LShift,
    RShift,
    Not
};

struct signal16;
std::map<std::string, signal16*> signals;

struct signal16 {
    source_type type{source_type::Value};
    unsigned short value{0};
    bool resolved{false};
    std::string a;
    std::string b;
    unsigned short argument{0};

    unsigned short get_a() const {
        bool has_only_digits = (a.find_first_not_of("0123456789") == std::string::npos);
        return has_only_digits ? std::stoi(a) : signals[a]->get_value();
    }

    unsigned short get_b() const {
        bool has_only_digits = (b.find_first_not_of("0123456789") == std::string::npos);
        return has_only_digits ? std::stoi(b) : signals[b]->get_value();
    }

    unsigned short get_value() {
        if (!resolved) {
            switch (type) {
                case source_type::And:
                    value = get_a() & get_b();
                    break;
                case source_type::Or:
                    value = get_a() | get_b();
                    break;
                case source_type::LShift:
                    value = get_a() << argument;
                    break;
                case source_type::RShift:
                    value = get_a() >> argument;
                    break;
                case source_type::Not:
                    value = ~get_a();
                    break;
                case source_type::Value:
                    value = get_a();
                    break;
            }
            resolved = true;
        }
        return value;
    }

    static signal16* new_value(std::string value) {
        auto x = new signal16;
        x->type = source_type::Value;
        x->a = value;
        return x;
    }

    static signal16* new_and(std::string a, std::string b) {
        auto x = new signal16;
        x->type = source_type::And;
        x->a = a;
        x->b = b;
        return x;
    }

    static signal16* new_or(std::string a, std::string b) {
        auto x = new signal16;
        x->type = source_type::Or;
        x->a = a;
        x->b = b;
        return x;
    }

    static signal16* new_lshift(std::string a, unsigned short argument) {
        auto x = new signal16;
        x->type = source_type::LShift;
        x->a = a;
        x->argument = argument;
        return x;
    }

    static signal16* new_rshift(std::string a, unsigned short argument) {
        auto x = new signal16;
        x->type = source_type::RShift;
        x->a = a;
        x->argument = argument;
        return x;
    }

    static signal16* new_not(std::string a) {
        auto x = new signal16;
        x->type = source_type::Not;
        x->a = a;
        return x;
    }
};

class day7 : public aoc::solution {
protected:

    void run(std::istream& in, std::ostream& out) override {

        std::regex and_re{R"(^(.*) AND (.*)$)"};
        std::regex or_re{R"(^(.*) OR (.*)$)"};
        std::regex lshift_re{R"(^(.*) LSHIFT (.*)$)"};
        std::regex rshift_re{R"(^(.*) RSHIFT (.*)$)"};
        std::regex not_re{R"(^NOT (.*)$)"};

        for (std::string line; std::getline(in, line);) {
            auto pieces = aoc::split_string(line, "->");
            auto signal_name = aoc::trim(pieces[1]);
            auto signal_source = aoc::trim(pieces[0]);

            std::smatch sm{};
            if (std::regex_match(signal_source, sm, and_re)) {
                auto a = aoc::trim(sm.str(1));
                auto b = aoc::trim(sm.str(2));
                signals[signal_name] = signal16::new_and(a, b);
            } else if (std::regex_match(signal_source, sm, or_re)) {
                auto a = aoc::trim(sm.str(1));
                auto b = aoc::trim(sm.str(2));
                signals[signal_name] = signal16::new_or(a, b);
            } else if (std::regex_match(signal_source, sm, lshift_re)) {
                auto a = aoc::trim(sm.str(1));
                auto amount = std::stoi(aoc::trim(sm.str(2)));
                signals[signal_name] = signal16::new_lshift(a, amount);
            } else if (std::regex_match(signal_source, sm, rshift_re)) {
                auto a = aoc::trim(sm.str(1));
                auto amount = std::stoi(aoc::trim(sm.str(2)));
                signals[signal_name] = signal16::new_rshift(a, amount);
            } else if (std::regex_match(signal_source, sm, not_re)) {
                auto a = aoc::trim(sm.str(1));
                signals[signal_name] = signal16::new_not(a);
            } else {
                signals[signal_name] = signal16::new_value(signal_source);
            }
        }

        // part 1
        auto part1 = signals["a"]->get_value();
        out << part1 << std::endl;

        // part 2
        for (auto& kv : signals) {
            kv.second->resolved = false;
        }
        signals["b"]->a = std::to_string(part1);
        auto part2 = signals["a"]->get_value();
        out << part2 << std::endl;
    }
};

int main(int argc, char** argv) {
    day7 solution;
    return solution.execute(argc, argv);
}
