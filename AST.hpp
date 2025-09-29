#include <string>
#include <memory>
#include <vector>



class ExpressionAST {
public:
    virtual ~ExpressionAST() = default;
};

class NumberExpressionAST : public ExpressionAST
{
    double Value;
public:
    NumberExpressionAST(double Val) : Value(Val) {}
};

class VariableExpressionAST : public ExpressionAST
{
    std::string Name;
public:
    VariableExpressionAST(const std::string &name) : Name(name) {}
};

class BinaryExpressionAST : public ExpressionAST
{
    char Op;
    std::unique_ptr<ExpressionAST> LHS,RHS;
public:
    BinaryExpressionAST(char Op,
                        std::unique_ptr<ExpressionAST> LHS,
                        std::unique_ptr<ExpressionAST> RHS
                        )
        : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExpressionAST : public ExpressionAST
{
    std::string Callee;
    std::vector<std::unique_ptr<ExpressionAST>> Args;
public:
    CallExpressionAST(std::string callee, std::vector<std::unique_ptr<ExpressionAST>> args)
    : Callee(callee), Args(std::move(args)) {} 
};

class PrototypeAST 
{
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(const std::string &name, std::vector<std::string> args)
    : Name(name), Args(std::move(args)) {}

    const std::string &getName() const {return Name;}
};

class FunctionAST 
{
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExpressionAST> Body;
public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto,
                std::unique_ptr<ExpressionAST> body)
    : Proto(std::move(proto)), Body(std::move(body)) {}
};
