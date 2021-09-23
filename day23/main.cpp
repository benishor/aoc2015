#include "solution.h"

enum instruction_type {
    hlf,
    tpl,
    inc,
    jmp,
    jie,
    jio
};

struct instruction {
    instruction_type type{hlf};
    uint8_t reg{0};
    int16_t offset{0};
};

struct cpu {
    uint32_t registers[2] = {0, 0};
    std::vector<instruction> code = {};
    int16_t ip{0};

    void run() {
        while (ip >= 0 && ip < code.size()) {
            execute_instruction(code.at(ip));
        }
    }

    void dump() {
        std::cout << "a = " << registers[0] << ", b = " << registers[1] << ", ip = " << ip << std::endl;
    }

    void execute_instruction(const instruction& i) {
        switch (i.type) {
            case hlf:
                registers[i.reg] >>= 1;
                ip++;
                break;
            case tpl:
                registers[i.reg] *= 3;
                ip++;
                break;
            case inc:
                registers[i.reg]++;
                ip++;
                break;
            case jmp:
                ip += i.offset;
                break;
            case jie:
                if ((registers[i.reg] & 1) == 0) {
                    ip += i.offset;
                } else {
                    ip++;
                }
                break;
            case jio:
                if (registers[i.reg] == 1) {
                    ip += i.offset;
                } else {
                    ip++;
                }
                break;
        }
    }
};

class day23 : public aoc::solution {
protected:
    void run(std::istream& in, std::ostream& out) override {

        cpu processor;

        for (std::string line; std::getline(in, line);) {
            auto opcode = line.substr(0, 3);
            if (opcode == "hlf") {
                processor.code.push_back(instruction{
                        .type = hlf,
                        .reg = static_cast<uint8_t>(line.at(4) - 'a'),
                });
            } else if (opcode == "tpl") {
                processor.code.push_back(instruction{
                        .type = tpl,
                        .reg = static_cast<uint8_t>(line.at(4) - 'a'),
                });
            } else if (opcode == "inc") {
                processor.code.push_back(instruction{
                        .type = inc,
                        .reg = static_cast<uint8_t>(line.at(4) - 'a'),
                });
            } else if (opcode == "jmp") {
                processor.code.push_back(instruction{
                        .type = jmp,
                        .offset = static_cast<int16_t>(std::stoi(line.substr(4))),
                });
            } else if (opcode == "jie") {
                processor.code.push_back(instruction{
                        .type = jie,
                        .reg = static_cast<uint8_t>(line.at(4) - 'a'),
                        .offset = static_cast<int16_t>(std::stoi(line.substr(7))),
                });
            } else if (opcode == "jio") {
                processor.code.push_back(instruction{
                        .type = jio,
                        .reg = static_cast<uint8_t>(line.at(4) - 'a'),
                        .offset = static_cast<int16_t>(std::stoi(line.substr(7))),
                });
            }
        }

        processor.run();
        std::cout << processor.registers[1] << std::endl;

        processor.registers[0] = 1;
        processor.registers[1] = 0;
        processor.ip = 0;
        processor.run();
        std::cout << processor.registers[1] << std::endl;
    }
};

int main(int argc, char **argv) {
    day23 solution{};
    return solution.execute(argc, argv);
}
