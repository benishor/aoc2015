#include "solution.h"

template<class F>
std::string regex_replace_with_callback(std::string_view haystack,
                                        const std::regex& rx, const F& f) {
    std::string result;
    const char* begin = haystack.data();
    const char* end = begin + haystack.size();
    std::cmatch m, lastm;
    if (!std::regex_search(begin, end, m, rx)) {
        return std::string(haystack);
    }

    do {
        lastm = m;
        result.append(m.prefix());
        result.append(f(m));
        begin = m[0].second;
        begin += (begin != end && m[0].length() == 0);
        if (begin == end) break;
    } while (std::regex_search(begin, end, m, rx,
                               std::regex_constants::match_prev_avail));
    result.append(lastm.suffix());
    return result;
}


class day8 : public aoc::solution {
protected:

    static int hex_char_to_nibble(char hexchar) {
        return (hexchar >= 'a') ? (hexchar - 'a' + 10) : (hexchar - '0');
    }

    void run(std::istream& in, std::ostream& out) override {

        std::regex slash_escape_re{R"(\\\\)"};
        std::regex quote_escape_re{R"(\\\")"};
        std::regex hex_escape_re{R"(\\x([0-9a-f])([0-9a-f]))"};

        size_t part1_code{0}, part1_mem{0}, part2_code{0};
        for (std::string line; std::getline(in, line);) {
            auto output = line.substr(1, line.size() - 2);

            output = std::regex_replace(output, slash_escape_re, "\\");
            output = std::regex_replace(output, quote_escape_re, "\"");
            output = regex_replace_with_callback(output, hex_escape_re,
                                                 [](auto&& m) {
                                                     auto val = (hex_char_to_nibble(m.str(1).at(0)) << 4 |
                                                                 hex_char_to_nibble(m.str(2).at(0)));
                                                     return std::string(1, (char) val);
                                                 });

            auto line_code_size = line.size();
            auto line_mem_size = output.size();
            part1_code += line_code_size;
            part1_mem += line_mem_size;

            size_t encoded_length = 2 + line.size()
                                    + std::count(line.begin(), line.end(), '"')
                                    + std::count(line.begin(), line.end(), '\\');
//            for (auto c : line) {
//                encoded_length++;
//                if (c == '"' || c == '\\') {
//                    encoded_length++;
//                }
//            }
            part2_code += encoded_length;

//            std::cout << line << " -> " << std::endl;
//            std::cout << line << " -> " << output << std::endl;
//            std::cout << "\tcode: " << line_code_size << std::endl;
//            std::cout << "\tmem: " << line_mem_size << std::endl;
//            std::cout << "\tencoded: " << encoded_length << std::endl;


        }
        out << (part1_code - part1_mem) << std::endl;
        out << (part2_code - part1_code) << std::endl;
    }

    void register_tests() override {
        add_test(aoc::test{
                .name = "First",
                .input = "\"\"\n\"abc\"\n\"aaa\\\"aaa\"\n\"\\x27\"",
                .expected_output = "12\n19"
        });
    }
};

int main(int argc, char** argv) {
    day8 solution;
    return solution.execute(argc, argv);
}
