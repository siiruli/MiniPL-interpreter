#pragma once
#include <map>
#include "Util.h"
#include "AstNode.h"
#include "Error.h"

/** @file */ 
struct variable {
  variable() : declared{1} {}
  bool declared = 0;
  ForAstNode *forParent;
};

class SemanticAnalyzer {
  public:
    SemanticAnalyzer(ErrorHandler &handler) : handler(handler) {}

    void visit(AstNode &node);

    void visit(ExprNode &node);
    void visit(LiteralNode &node);
    void visit(UnaryOp &node);
    void visit(BinaryOp &node);
    void visit(VarNode &node);
    

    void visit(IfAstNode &node);
    void visit(DeclAstNode &node);
    void visit(AssignAstNode &node);
    void visit(ForAstNode &node);
    void visit(PrintAstNode &node);
    void visit(ReadAstNode &node);
    void visit(StatementsAstNode &node);

  private:
    ErrorHandler &handler;
    
    std::vector<Span> scopes;
    

    template<class NodeType> 
    void raiseError(NodeType &node);
    
    template<class NodeType> 
    void raiseError(NodeType &node, SemanticError error);

    template<class NodeType> 
    void raiseError(NodeType &node, VarIdent varId, SemErrorType type);


    std::map<VarIdent, variable> variables; 

    // use unique_ptr

    void initVar(VarIdent varId);
    bool hasVar(VarIdent varId);
    variable & getVar(VarIdent varId);

    template<class NodeType> 
    void declareVar(NodeType &node, VarIdent varId);
    template<class NodeType> 
    void assignVar(NodeType &node, VarIdent varId);
    template<class NodeType> 
    void accessVar(NodeType &node, VarIdent varId);
    

};
