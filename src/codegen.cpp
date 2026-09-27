#include "codegen.hpp"

#include "asm_ast.hpp"
#include "ast.hpp"

x86::Program gen_program(const Program &program) {

    x86::Function fun = gen_function(program.function);
    x86::Program prog;
    prog.function = fun;

    return prog;
}

x86::Function gen_function(const Function &function) {

    std::string name = function.name;
    std::vector<x86::Instruction> body = gen_return(function.body);

    x86::Function fun;
    fun.name = name;
    fun.instructions = body;

    return fun;
}

std::vector<x86::Instruction> gen_return(const Return &ret) {

    std::vector<x86::Instruction> vec;
    x86::Operand op = gen_constant(ret.constant);

    vec.push_back(x86::Mov{op, x86::Register{}});
    vec.push_back(x86::Ret{});
    return vec;
}

x86::Operand gen_constant(const Constant &constant) { return x86::Imm{constant.value}; }
