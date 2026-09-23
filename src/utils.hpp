
#pragma once

#include <fstream>
#include <iostream>

#include "lexer.hpp"

std::ostream &operator<<(std::ostream &os, const TokenType &type) {

    switch (type) {
    case TokenType::IDENTIFIER:
        return os << "IDENTIFIER";
    case TokenType::INTEGER_CONSTANT:
        return os << "INTEGER";
    case TokenType::KEYWORD:
        return os << "KEYWORD";
    case TokenType::PUNCTUATORS:
        return os << "PUNCTUATORS";
    case TokenType::SEMICOLON:
        return os << "SEMICOLON";
    case TokenType::UNKNOWN:
        return os << "UNKNOWN";
    }
}
