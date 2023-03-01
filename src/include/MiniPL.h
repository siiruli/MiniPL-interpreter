#include "Scanner.h"
#include "Parser.h"
#include "AstNode.h"
#include "InterpreterVisitor.h"

class MiniPL {
  public:
    void run(std::string program);
};