#pragma once

#include <string>

struct Constant {
    int value;
};

struct Return {
    Constant constant;
};

struct Function {
    std::string name;
    Return body;
};

struct Program {
    Function function;
};
