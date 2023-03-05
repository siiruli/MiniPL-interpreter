#include "Scanner.h"
#include "Parser.h"
#include "AstNode.h"
#include "InterpreterVisitor.h"

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

    void run(std::string program);

  private:
    std::istream &input;
    std::ostream &output;
};