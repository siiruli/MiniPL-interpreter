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

    Type visit(AstNode &node);
    
    Type visit(ExprNode &node);
    Type visit(UnaryOp &node);
    Type visit(BinaryOp &node);
    Type visit(VarNode &node);
        
    Type visit(IfAstNode &node);
    Type visit(DeclAstNode &node);
    Type visit(AssignAstNode &node);
    Type visit(ForAstNode &node);
    Type visit(PrintAstNode &node);
    Type visit(ReadAstNode &node);
    Type visit(StatementsAstNode &node);
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
