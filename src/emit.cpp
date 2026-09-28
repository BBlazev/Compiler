#include <variant>

#include "emit.hpp"

void emit(std::ostream &os, const x86::Program &prog) {
    emit(os, prog.function);
    os << "\t.section .note.GNU-stack,\"\",@progbits\n";
}

void emit(std::ostream &os, const x86::Function &func) {
    os << "\t.globl " << func.name << "\n";
    os << func.name << ":\n";
    for (const auto &inst : func.instructions)
        emit(os, inst);
}

void emit(std::ostream &os, const x86::Instruction &inst) {
    if (const auto *mov = std::get_if<x86::Mov>(&inst)) {
        os << "\tmovl ";
        emit(os, mov->src);
        os << ", ";
        emit(os, mov->dst);
        os << "\n";
    } else if (std::holds_alternative<x86::Ret>(inst)) {
        os << "\tret\n";
    }
}

void emit(std::ostream &os, const x86::Operand &op) {
    if (const auto *imm = std::get_if<x86::Imm>(&op))
        os << "$" << imm->value;
    else if (std::holds_alternative<x86::Register>(op))
        os << "%eax";
}
