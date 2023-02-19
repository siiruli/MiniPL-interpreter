#include <map>
#include "Visitor.h"
#include "AstNode.h"

enum Type {Int, Bool, String};

class InterpreterVisitor : Visitor {
  public:
    void visitIf(IfAstNode &node);
    void visitDecl(DeclAstNode &node);
    void visitAssign(AssignAstNode &node);
    void visitFor(ForAstNode & node);
    void visitPrint(PrintAstNode & node);
    void visitRead(ReadAstNode & node);
  private:
  
    std::map<std::string, int> variables; 

    template<typename T> 
    void setValue(std::string varId, T value);
    

    void init(int type, std::string varId);
    int getType(std::string varId);
    int getValue(std::string varId);
    
};
