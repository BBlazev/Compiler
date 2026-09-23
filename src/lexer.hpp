
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType {

    IDENTIFIER,
    KEYWORD,
    INTEGER_CONSTANT,
    OPEN,
    CLOSE,
    SEMICOLON,
    UNKNOWN

};

struct Token {

    TokenType type;
    std::string value;

    Token(TokenType t, const std::string &v) : type(t), value(v) {}
};

class Lexer {

  public:
    Lexer();
    ~Lexer();

    Lexer(const Lexer &) = delete;
    Lexer(Lexer &&) = delete;
    Lexer &operator=(const Lexer &) = delete;
    Lexer &operator=(Lexer &&) = delete;

    std::vector<Token> lex(std::string source);

  private:
    std::string input;
    std::size_t position;
    std::unordered_map<std::string, TokenType> keywords;
    std::unordered_map<std::string, TokenType> punctuator_open;
    std::unordered_map<std::string, TokenType> punctuator_closed;

    void initKeywords() noexcept;
    bool isWhiteSpace(char c) noexcept;
    bool isAlpha(char c) noexcept;
    bool isDigit(char c) noexcept;
    bool isAlphaNumeric(char c) noexcept;
    std::string getNextWord();
    std::string getNextNumber();
};
