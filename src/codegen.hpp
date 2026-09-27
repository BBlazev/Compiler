#pragma once
#include "asm_ast.hpp"
#include "ast.hpp"

x86::Program gen_program(const Program &program);
x86::Function gen_function(const Function &function);
std::vector<x86::Instruction> gen_return(const Return &ret);
x86::Operand gen_constant(const Constant &constant);
