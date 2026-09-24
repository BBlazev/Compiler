#pragma once

#include <cstddef>
#include <vector>

#include "ast.hpp"
#include "lexer.hpp"

class Parser {
  public:
    Parser(std::vector<Token> &tokens_);
    Program parse_program();

  private:
    const Token &peek();
    Token advance();
    Token expect(TokenType type, const std::string &text);
    Token expect(TokenType type);

    Constant parse_expression();
    Return parse_statement();
    Function parse_function();

    std::vector<Token> tokens;
    std::size_t position;
};
