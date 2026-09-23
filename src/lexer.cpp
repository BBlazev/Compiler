#include <cstddef>
#include <iostream>
#include <stdexcept>

#include "lexer.hpp"

static int line_number = 1;
static int column_number = 1;

Lexer::Lexer(const std::string &src) : input(src), position(0) { init(); }

Lexer::~Lexer() {}

void Lexer::init() noexcept {

    keywords["int"] = TokenType::KEYWORD;
    keywords["float"] = TokenType::KEYWORD;
    keywords["if"] = TokenType::KEYWORD;
    keywords["else"] = TokenType::KEYWORD;
    keywords["while"] = TokenType::KEYWORD;
    keywords["return"] = TokenType::KEYWORD;
    keywords["void"] = TokenType::KEYWORD;
}

bool Lexer::isWhiteSpace(char c) noexcept {
    if (c == '\n') {
        line_number++;
        column_number = 0;
        return c;
    }
    return c == ' ' || c == '\t' || c == '\r';
}

bool Lexer::isDigit(char c) noexcept { return c >= '0' && c <= '9'; }

bool Lexer::isAlpha(char c) noexcept {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool Lexer::isAlphaNumeric(char c) noexcept { return isAlpha(c) || isDigit(c); }

std::string Lexer::getNextWord() noexcept {

    std::size_t start = position;

    while (position < input.length() && isAlphaNumeric(input[position])) {
        position++;
        column_number++;
    }

    return input.substr(start, position - start);
}

std::string Lexer::getNextNumber() noexcept {

    size_t start = position;
    bool hasDecimal = false;

    while (position < input.length() && (isDigit(input[position]) || input[position] == '.')) {
        if (input[position] == '.') {
            if (hasDecimal)
                break;
            hasDecimal = true;
        }
        column_number++;
        position++;
    }
    return input.substr(start, position - start);
}

char Lexer::peek() noexcept { return input[position + 1]; }

std::vector<Token> Lexer::lex() {

    std::vector<Token> tokens;

    while (position < input.length()) {

        char currentChar = input[position];
        int col = column_number;
        if (isWhiteSpace(currentChar)) {
            position++;
            column_number++;
            continue;
        }

        else if (isAlpha(currentChar)) {
            std::string word = getNextWord();
            if (keywords.find(word) != keywords.end()) {
                tokens.emplace_back(TokenType::KEYWORD, std::move(word), line_number, col);
            } else {
                tokens.emplace_back(TokenType::IDENTIFIER, std::move(word), line_number, col);
            }
        }

        else if (isDigit(currentChar)) {
            std::string num = getNextNumber();
            if (isAlpha(input[position])) {
                num += getNextWord();
                throw std::runtime_error("Error\n");
            } else if (num.find('.') != std::string::npos) {
                tokens.emplace_back(TokenType::FLOAT_CONSTANT, std::move(num), line_number, col);
            } else {
                tokens.emplace_back(TokenType::INTEGER_CONSTANT, std::move(num), line_number, col);
            }
        }

        else if (currentChar == '+' || currentChar == '-' || currentChar == '*' ||
                 currentChar == '/' || currentChar == '=') {
            char next = peek();

            if (next == '+' || next == '-' || next == '*' || next == '/' || next == '=') {
                std::string s = std::string(1, currentChar) + next;
                tokens.emplace_back(TokenType::DOUBLE_OPERATOR, std::move(s), line_number, col);
                position += 2;
                column_number += 2;
            } else {
                tokens.emplace_back(TokenType::OPERATOR, std::string(1, currentChar), line_number,
                                    col);
                position++;
                column_number++;
                {
                }
            }
        }

        else if (currentChar == '(' || currentChar == ')' || currentChar == '{' ||
                 currentChar == '}' || currentChar == ';') {
            tokens.emplace_back(TokenType::PUNCTUATORS, std::string(1, currentChar), line_number,
                                col);
            position++;
            column_number++;
        }

        // else if (currentChar == '\n') {
        //    column_number = 1;
        //   line_number++;
        //}

        else {
            throw std::runtime_error(std::to_string(line_number) + ":" + std::to_string(col) +
                                     ": Unknown character '" + currentChar + "'\n");
            //	tokens.emplace_back(TokenType::UNKNOWN, std::string(1, currentChar));
            //	position++;
        }
    }

    return tokens;
}
