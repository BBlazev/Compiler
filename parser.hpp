#include "lexer.hpp"
#include "AST.hpp"
#include <memory>
#include <map>

static std::map<char,int> BinaryPrecedence;
static int CurrentToken;
static int getNextToken() { return CurrentToken = getToken();}

static std::unique_ptr<ExpressionAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExpressionAST> LHS)
{
    while(true)
    {
        int TokenPrec = GetTokenPrecedence();    
        if(TokenPrec < ExprPrec) return LHS;
    }
}



static std::unique_ptr<ExpressionAST> ParseExpression()
{
    auto LHS = ParsePrimary();
    if(!LHS) return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}


static std::unique_ptr<ExpressionAST> ParseNumberExpression()
{
    auto Result = std::make_unique<NumberExpressionAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

static std::unique_ptr<ExpressionAST> ParseParenthesisExpression()
{
    getNextToken();
    auto V = ParseExpression();
    if(!V) return nullptr;

    if(CurrentToken != ')') return LogError("expected ')'");
    getNextToken();
    return V;
}

static std::unique_ptr<ExpressionAST> ParseIdentifierExpression()
{
    std::string IdName = IdentifierStr;
    
    getNextToken();

    if(CurrentToken != '(') return std::make_unique<VariableExpressionAST>(IdName);

    getNextToken();
    std::vector<std::unique_ptr<ExpressionAST>> Args;
    if(CurrentToken != ')')
    {
        while (true)
        {
            if(auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else return nullptr;
            
            if(CurrentToken == ')') break;
            if(CurrentToken != ',') return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
        
    }
    getNextToken();
    return std::make_unique<CallExpressionAST>(IdName, std::move(Args));
}

static std::unique_ptr<ExpressionAST> ParsePrimary()
{
    switch(CurrentToken)
    {
        default:
        return LogError("unknown token when expecting expression");
        case token_identifier:
            return ParseIdentifierExpression();
        case token_number:
            return ParseNumberExpression();
        case '(':
            return ParseParenthesisExpression();
    }
}

static int GetTokenPrecedence()
{
    if(!isascii(CurrentToken)) return -1;

    int TokenPrec = BinaryPrecedence[CurrentToken];
    if(TokenPrec <= 0) return -1;
    return TokenPrec;
}










std::unique_ptr<ExpressionAST> LogError(const char *str)
{
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}
std::unique_ptr<PrototypeAST> LogError_Prototype(const char *str)
{
    LogError(str);
    return nullptr;
}