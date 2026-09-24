#include <exception>
#include <stdexcept>
#include <string>

#include "parser.hpp"

#include "ast.hpp"
#include "lexer.hpp"

Parser::Parser(std::vector<Token> &tokens_) : tokens(tokens_), position(0) {}

const Token &Parser::peek() {

    if (position >= tokens.size())
        throw std::runtime_error("Unexpected end of file");
    return tokens[position];
}

Token Parser::advance() {

    const Token &current = peek();
    position++;
    return current;
}

Token Parser::expect(TokenType type, const std::string &text) {

    const Token &current = peek();
    if (current.type == type && current.value == text) {
        return advance();
    }
    throw std::runtime_error(std::to_string(current.line) + ":" +
                             std::to_string(current.column_number) + ": expected '" + text +
                             "' but found '" + current.value + "'");
}

Token Parser::expect(TokenType type) {
    const Token &current = peek();
    if (current.type == type)
        return advance();

    throw std::runtime_error(std::to_string(current.line) + ":" +
                             std::to_string(current.column_number) +
                             " expected identifier but found '" + current.value + "'");
}

Constant Parser::parse_expression() {

    Token current = expect(TokenType::INTEGER_CONSTANT);

    Constant con;
    con.value = std::stoi(current.value);

    return con;
}

Return Parser::parse_statement() {

    expect(TokenType::RETURN);
    Constant con = parse_expression();
    expect(TokenType::PUNCTUATORS, ";");

    Return ret;
    ret.constant = con;

    return ret;
}

Function Parser::parse_function() {

    expect(TokenType::KEYWORD);
    Token current = expect(TokenType::IDENTIFIER);
    std::string name = current.value;

    expect(TokenType::PUNCTUATORS);
    expect(TokenType::KEYWORD);
    expect(TokenType::PUNCTUATORS);
    expect(TokenType::PUNCTUATORS);

    Return ret = parse_statement();
    expect(TokenType::PUNCTUATORS);

    Function fun;
    fun.name = name;
    fun.body = ret;

    return fun;
}

Program Parser::parse_program() {

    Function fun;
    fun = parse_function();

    if (position != tokens.size()) {
        throw std::runtime_error("Leftover tokens at parser");
    }
    Program program;
    program.function = fun;

    return program;
}
