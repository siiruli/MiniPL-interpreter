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
    Type visit(IfAstNode &node);
    Type visit(DeclAstNode &node);
    Type visit(AssignAstNode &node);
    Type visit(ForAstNode &node);
    Type visit(PrintAstNode &node);
    Type visit(ReadAstNode &node);
    Type visit(StatementsAstNode &node);
    Type visit(ExprAstNode &node);
    Type visit(OpndAstNode &node);
  private:
    ErrorHandler &handler;
    
    template<class NodeType> 
    void raiseError(NodeType &node, Types exp, Types got, Span span);
    template<class NodeType> 
    void raiseError(NodeType &node, TypeError error);
    
    std::map<std::string, Type> variables; 

    // use unique_ptr

    void initVar(Type type, std::string varId);

    void setVar(std::string varId, Type type);
    Type &getVar(std::string varId);
    Type getVal(std::string varId);


    

};
