#include <iostream>
#include "MiniPL.h"


void MiniPL::run(std::string program){
  ErrorHandler handler;
  Scanner scanner(program, handler);
  ScannerIterator it(scanner); 
  Parser parser(it, handler);

  AstNode ast = parser.program();
  if(handler.hasErrors()){
    std::cout << "Errors while scanning/parsing. Ending process.\n";
    return;
  }

  InterpreterVisitor visitor{input, output};
  visitor.visit(ast);
}