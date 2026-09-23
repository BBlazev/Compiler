#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType {

    IDENTIFIER,
    KEYWORD,
    INTEGER_CONSTANT,
    FLOAT_CONSTANT,
    PUNCTUATORS,
    SEMICOLON,
    OPERATOR,
    UNKNOWN

};

struct Token {

    TokenType type;
    std::string value;
    int line;
    int column_number;

    Token(TokenType t, const std::string &v, int ln, int col)
        : type(t), value(v), line(ln), column_number(col) {}
};

class Lexer {

  public:
    Lexer(const std::string &src);
    ~Lexer();

    Lexer(const Lexer &) = delete;
    Lexer(Lexer &&) = delete;
    Lexer &operator=(const Lexer &) = delete;
    Lexer &operator=(Lexer &&) = delete;

    std::vector<Token> lex();

  private:
    std::string input;
    std::size_t position;
    std::unordered_map<std::string, TokenType> keywords;

    void init() noexcept;
    bool isWhiteSpace(char c) noexcept;
    bool isAlpha(char c) noexcept;
    bool isDigit(char c) noexcept;
    bool isAlphaNumeric(char c) noexcept;
    std::string getNextWord() noexcept;
    std::string getNextNumber() noexcept;
};
