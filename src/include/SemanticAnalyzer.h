#include <set>
#include "Util.h"
#include "AstNode.h"
#include "Error.h"

#pragma once

class SemanticAnalyzer {
  public:
    SemanticAnalyzer(ErrorHandler &handler) : handler(handler) {}

    void visit(AstNode &node);
    void visit(IfAstNode &node);
    void visit(DeclAstNode &node);
    void visit(AssignAstNode &node);
    void visit(ForAstNode &node);
    void visit(PrintAstNode &node);
    void visit(ReadAstNode &node);
    void visit(StatementsAstNode &node);
    void visit(ExprAstNode &node);
    void visit(OpndAstNode &node);
  private:
    ErrorHandler &handler;
    
    template<class NodeType> 
    void raiseError(NodeType &node);

    std::set<VarIdent> variables; 

    // use unique_ptr

    void initVar(VarIdent varId);
    bool hasVar(VarIdent varId);

    

};
