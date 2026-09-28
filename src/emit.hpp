#pragma once

#include <ostream>

#include "asm_ast.hpp"

void emit(std::ostream &os, const x86::Program &prog);
void emit(std::ostream &os, const x86::Function &func);
void emit(std::ostream &os, const x86::Instruction &inst);
void emit(std::ostream &os, const x86::Operand &op);
