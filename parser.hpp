#pragma once

#include "lexer.hpp"
#include "AST.hpp"
#include <memory>
#include <map>

static std::map<char, int> BinaryPrecedence;
static int CurrentToken;

static std::unique_ptr<ExpressionAST> ParseExpression();
static std::unique_ptr<ExpressionAST> ParsePrimary();
static std::unique_ptr<ExpressionAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExpressionAST> LHS);
static std::unique_ptr<PrototypeAST> ParsePrototype();

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

static int getNextToken()
{
    return CurrentToken = getToken();
}

static int GetTokenPrecedence()
{
    if (!isascii(CurrentToken))
        return -1;

    int TokenPrec = BinaryPrecedence[CurrentToken];
    if (TokenPrec <= 0)
        return -1;
    return TokenPrec;
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
    if (!V)
        return nullptr;

    if (CurrentToken != ')')
        return LogError("expected ')'");
    getNextToken();
    return V;
}

static std::unique_ptr<ExpressionAST> ParseIdentifierExpression()
{
    std::string IdName = IdentifierStr;
    getNextToken();

    if (CurrentToken != '(')
        return std::make_unique<VariableExpressionAST>(IdName);

    getNextToken();
    std::vector<std::unique_ptr<ExpressionAST>> Args;
    if (CurrentToken != ')')
    {
        while (true)
        {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurrentToken == ')')
                break;
            if (CurrentToken != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }
    getNextToken();
    return std::make_unique<CallExpressionAST>(IdName, std::move(Args));
}

static std::unique_ptr<ExpressionAST> ParsePrimary()
{
    switch (CurrentToken)
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

static std::unique_ptr<ExpressionAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExpressionAST> LHS)
{
    while (true)
    {
        int TokenPrec = GetTokenPrecedence();
        if (TokenPrec < ExprPrec)
            return LHS;
    }
}

static std::unique_ptr<ExpressionAST> ParseExpression()
{
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

static std::unique_ptr<PrototypeAST> ParsePrototype()
{
    if (CurrentToken != token_identifier)
        return LogError_Prototype("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurrentToken != '(')
        return LogError_Prototype("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (getNextToken() == token_identifier)
        ArgNames.push_back(IdentifierStr);

    if (CurrentToken != ')')
        return LogError_Prototype("Expected ')' in prototype");

    getNextToken();
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

static std::unique_ptr<FunctionAST> ParseDefinition()
{
    getNextToken();
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));

    return nullptr;
}

static std::unique_ptr<PrototypeAST> ParseExtern()
{
    getNextToken();
    return ParsePrototype();
}

static std::unique_ptr<FunctionAST> ParseTopLevelExpression()
{
    if (auto E = ParseExpression())
    {
        auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}