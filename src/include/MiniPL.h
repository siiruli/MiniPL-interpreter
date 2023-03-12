#include "Scanner.h"
#include "Parser.h"
#include "AstNode.h"
#include "InterpreterVisitor.h"

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

    void runProgram(std::string &program);
    void runFile(std::string fileName);
    void run(std::vector<std::string> &program);

  private:
    std::istream &input;
    std::ostream &output;
};