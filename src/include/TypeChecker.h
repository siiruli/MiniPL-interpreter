#pragma once
#include <map>
#include "Util.h"
#include "AstNode.h"
#include "Error.h"

/** @file */ 

typedef std::vector<Type> Types;

class TypeChecker {
  public:
    TypeChecker(ErrorHandler &handler) : handler(handler) {}

    void visit(StatementNode &node);
    
    Type visit(ExprNode &node);
    Type visit(UnaryOp &node);
    Type visit(BinaryOp &node);
    Type visit(VarNode &node);
        
    void visit(IfAstNode &node);
    void visit(DeclAstNode &node);
    void visit(AssignAstNode &node);
    void visit(ForAstNode &node);
    void visit(PrintAstNode &node);
    void visit(ReadAstNode &node);
    void visit(StatementsAstNode &node);
  private:
    ErrorHandler &handler;
    
    template<class NodeType> 
    void raiseError(NodeType &node, Types exp, Types got, Span span);
    template<class NodeType> 
    void raiseError(NodeType &node, TypeError error);
    
    std::map<std::string, Type> variables; 

    // use unique_ptr
    template<class NT>
    inline Span getSpan(NT &node){
      return std::visit([](auto &node) -> Span {
        return node.span;
      }, node);
    }
    void setType(std::string varId, Type type);
    Type getType(std::string varId);

    template<class NodeType> 
    Type match(Type type1, Type type2, NodeType &node, Span span);
    

};
