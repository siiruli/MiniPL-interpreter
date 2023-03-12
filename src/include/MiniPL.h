#include "Scanner.h"
#include "Parser.h"
#include "AstNode.h"
#include "Interpreter.h"

/** @file */ 
class MiniPL {
  public:

    MiniPL() : 
      input{std::cin}, 
      output{std::cout} 
    {}

    MiniPL(std::istream &in, std::ostream &out) : 
      input{in}, 
      output{out} 
    {}

    void runFile(std::string filename);
    void runFile(std::string filename, ErrorHandler &handler);
    void run(Program &program);
    void run(Program &program, ErrorHandler &handler);

  private:
    std::istream &input;
    std::ostream &output;
};