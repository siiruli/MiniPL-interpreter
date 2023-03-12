#include <iostream>
#include <fstream>
#include "MiniPL.h"
#include "SemanticAnalyzer.h"
#include "TypeChecker.h"

void MiniPL::runFile(std::string filename){
  ErrorHandler handler{};
  runFile(filename, handler);
}

void MiniPL::runFile(std::string filename, ErrorHandler &handler){
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
  run(program, handler);
}

void MiniPL::run(Program &program){
  ErrorHandler handler{};
  run(program, handler);
}

void MiniPL::run(Program &program, ErrorHandler &handler){
  Scanner scanner(program, handler);
  ScannerIterator it(scanner); 
  Parser parser(it, handler);

  AstNode ast = parser.program();
  if(handler.hasErrors()){
    output << "Errors while scanning/parsing. Ending process.\n";
    handler.printErrors(program, output);
    return;
  }
  SemanticAnalyzer analyzer(handler);
  analyzer.visit(ast);
  
  TypeChecker typeChecker(handler);
  typeChecker.visit(ast);

  if(handler.hasErrors()){
    output << "Found semantic errors. Ending process.\n";
    handler.printErrors(program, output);
    return;
  }


  InterpreterVisitor visitor{input, output, handler};
  try {
    visitor.visit(ast);
  } catch (RunTimeException &e){
    output << "Runtime error: " << e.what() << std::endl;
  } catch (std::ios_base::failure &e){
    output << "IO error: " << e.what() << std::endl;
  }

  if(handler.hasErrors()){
    handler.printErrors(program, output);
  }




}