#pragma once

#include "parser.hpp"

static void HandleDefinition()
{
    if(ParseDefinition())
        fprintf(stderr, "Parsed a function definition. \n");
    else getNextToken();
}

static void HandleExtern()
{
    if(ParseExtern())
        fprintf(stderr, "Parsed an extern. \n");
    else getNextToken();
}

static void HandleTopLevelExpression()
{
    if(ParseTopLevelExpression())
        fprintf(stderr, "Parsed top-level expression. \n");
    getNextToken();
}


static void MainLoop()
{
    while(true)
    {
        fprintf(stderr, "ready> ");
        switch (CurrentToken)
        {
            case token_eof:
                return;
            case ';':
                getNextToken();
                break;
            case token_def:
                HandleDefinition();
                break;
            case token_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}