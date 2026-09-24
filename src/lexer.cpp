#include <cstddef>
#include <stdexcept>

#include "lexer.hpp"

Lexer::Lexer(const std::string &src) : input(src), position(0) { init(); }

Lexer::~Lexer() {}

void Lexer::init() noexcept {

    keywords["int"] = TokenType::KEYWORD;
    keywords["float"] = TokenType::KEYWORD;
    keywords["if"] = TokenType::KEYWORD;
    keywords["else"] = TokenType::KEYWORD;
    keywords["while"] = TokenType::KEYWORD;
    keywords["return"] = TokenType::RETURN;
    keywords["void"] = TokenType::KEYWORD;

    double_operators["=="] = TokenType::DOUBLE_OPERATOR;
    double_operators["<="] = TokenType::DOUBLE_OPERATOR;
    double_operators[">="] = TokenType::DOUBLE_OPERATOR;
    double_operators["!="] = TokenType::DOUBLE_OPERATOR;
    double_operators["<<"] = TokenType::DOUBLE_OPERATOR;
    double_operators[">>"] = TokenType::DOUBLE_OPERATOR;
    double_operators["++"] = TokenType::DOUBLE_OPERATOR;
    double_operators["--"] = TokenType::DOUBLE_OPERATOR;
    double_operators["-="] = TokenType::DOUBLE_OPERATOR;
    double_operators["+="] = TokenType::DOUBLE_OPERATOR;
}

void Lexer::advance() noexcept {

    if (input[position] == '\n') {
        line_number++;
        column_number = 0;
    }

    position++;
    column_number++;
}

bool Lexer::isWhiteSpace(char c) noexcept {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool Lexer::isDigit(char c) noexcept { return c >= '0' && c <= '9'; }

bool Lexer::isAlpha(char c) noexcept {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool Lexer::isAlphaNumeric(char c) noexcept { return isAlpha(c) || isDigit(c); }

std::string Lexer::getNextWord() noexcept {

    std::size_t start = position;

    while (position < input.length() && isAlphaNumeric(input[position])) {
        advance();
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
        advance();
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
            advance();
            continue;
        }

        else if (isAlpha(currentChar)) {
            std::string word = getNextWord();
            auto it = keywords.find(word);
            if (it != keywords.end()) {
                TokenType type = it->second;
                tokens.emplace_back(type, std::move(word), line_number, col);
            } else {
                tokens.emplace_back(TokenType::IDENTIFIER, std::move(word), line_number, col);
            }
        }

        else if (isDigit(currentChar)) {
            std::string num = getNextNumber();
            if (isAlpha(input[position])) {
                num += getNextWord();
                throw std::runtime_error(std::to_string(line_number) + ":" + std::to_string(col) +
                                         ": Cant start with number " + num + "\n");
            } else if (num.find('.') != std::string::npos) {
                tokens.emplace_back(TokenType::FLOAT_CONSTANT, std::move(num), line_number, col);
            } else {
                tokens.emplace_back(TokenType::INTEGER_CONSTANT, std::move(num), line_number, col);
            }
        }

        else if (currentChar == '+' || currentChar == '-' || currentChar == '*' ||
                 currentChar == '/' || currentChar == '=' || currentChar == '<' ||
                 currentChar == '>' || currentChar == '!') {

            char next = peek();
            std::string s = std::string{currentChar, next};

            if (double_operators.find(s) != double_operators.end()) {
                tokens.emplace_back(TokenType::DOUBLE_OPERATOR, std::move(s), line_number, col);
                advance();
                advance();

            } else {
                tokens.emplace_back(TokenType::OPERATOR, std::string(1, currentChar), line_number,
                                    col);
                advance();
            }
        }

        else if (currentChar == '(' || currentChar == ')' || currentChar == '{' ||
                 currentChar == '}' || currentChar == ';') {
            tokens.emplace_back(TokenType::PUNCTUATORS, std::string(1, currentChar), line_number,
                                col);
            advance();
        }

        else {
            throw std::runtime_error(std::to_string(line_number) + ":" + std::to_string(col) +
                                     ": Unknown character '" + currentChar + "'\n");
            //	tokens.emplace_back(TokenType::UNKNOWN, std::string(1, currentChar));
            //	position++;
        }
    }

    return tokens;
}
