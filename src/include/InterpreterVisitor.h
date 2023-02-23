#include <map>
#include "Util.h"
#include "Visitor.h"
#include "AstNode.h"



class InterpreterVisitor : Visitor {
  public:
    void visit(AstNode &node);
    void visit(IfAstNode &node);
    void visit(DeclAstNode &node);
    void visit(AssignAstNode &node);
    void visit(ForAstNode &node);
    void visit(PrintAstNode &node);
    void visit(ReadAstNode &node);
    void visit(ExprAstNode &node);
    void visit(StatementsAstNode &node);
    void visit(ErrorAstNode &node);
  private:
  
    std::map<std::string, ExprValue> variables; 

    // use unique_ptr

    void initVar(Type type, std::string varId);
    void setVar(std::string varId, ExprValue val);
    ExprValue &getVar(std::string varId);
    
};
