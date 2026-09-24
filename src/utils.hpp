
#pragma once

#include <iostream>

#include "lexer.hpp"

inline std::ostream &operator<<(std::ostream &os, const TokenType &type) {

    switch (type) {
    case TokenType::IDENTIFIER:
        return os << "IDENTIFIER";
    case TokenType::INTEGER_CONSTANT:
        return os << "INTEGER";
    case TokenType::FLOAT_CONSTANT:
        return os << "FLOAT_CONSTANT";
    case TokenType::OPERATOR:
        return os << "OPERATOR";
    case TokenType::DOUBLE_OPERATOR:
        return os << "DOUBLE_OPERATOR";
    case TokenType::KEYWORD:
        return os << "KEYWORD";
    case TokenType::PUNCTUATORS:
        return os << "PUNCTUATORS";
    case TokenType::SEMICOLON:
        return os << "SEMICOLON";
    case TokenType::RETURN:
        return os << "RETURN";
    }

    return os << "UNKNOWN";
}
