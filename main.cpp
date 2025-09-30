#include "top_level_parser.hpp"
#include "parser.hpp"

int main()
{

  BinaryPrecedence['<'] = 10;
  BinaryPrecedence['+'] = 20;
  BinaryPrecedence['-'] = 20;
  BinaryPrecedence['*'] = 40; 

  fprintf(stderr, "ready> ");
  getNextToken();

  MainLoop();

  return 0;
}