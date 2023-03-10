#include <iostream>
#include <fstream>
#include "MiniPL.h"
#include "TypeChecker.h"

void MiniPL::runProgram(std::string &program){

}
void MiniPL::runFile(std::string filename){
  std::ifstream file(filename);
  std::vector<std::string> program;
  if(!file.is_open()){
    std::cout << "Unable to open file: " << filename << std::endl;
    return;
  }
  std::string line;
  while(std::getline(file, line)){
    program.push_back(line + "\n");
  }
  run(program);
}
void MiniPL::run(std::vector<std::string> &program){
  ErrorHandler handler(program);
  Scanner scanner(program, handler);
  ScannerIterator it(scanner); 
  Parser parser(it, handler);

  AstNode ast = parser.program();
  if(handler.hasErrors()){
    std::cout << "Errors while scanning/parsing. Ending process.\n";
    return;
  }
  TypeChecker typeChecker(handler);
  typeChecker.visit(ast);

  if(handler.hasErrors()){
    std::cout << "Found type errors. Ending process.\n";
    return;
  }


  InterpreterVisitor visitor{input, output};
  visitor.visit(ast);
}