#pragma once

#include <string>
#include <variant>
#include <vector>

namespace x86 {

struct Imm {
    int value;
};

struct Register {};

using Operand = std::variant<Imm, Register>;

struct Mov {
    Operand src;
    Operand dst;
};

struct Ret {};

using Instruction = std::variant<Mov, Ret>;

struct Function {
    std::string name;
    std::vector<Instruction> instructions;
};

struct Program {
    Function function;
};

} // namespace x86
