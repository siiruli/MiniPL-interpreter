#include <iostream>
#include <assert.h>
#include "SemanticAnalyzer.h"


template<class NodeType> 
void SemanticAnalyzer::raiseError(NodeType &node){
  SemanticError error;
  error.span = node.span;
  error.context = nodenames[AstNode(std::in_place_type<NodeType>).index()];
  handler.raiseError(error);
}

void SemanticAnalyzer::visit(ExprAstNode &node){

  switch (node.op)
  {
    case Operator::Identity:
    case Operator::Not: 
      if(node.opnd2) raiseError(node);
      break;
    default:
      if(!node.opnd2) raiseError(node);
      break;
  }

  visit(*node.opnd1);
  
  if(node.opnd2) {
    visit(*node.opnd2);
  }

}

void SemanticAnalyzer::visit(OpndAstNode &node){
  std::visit( overloaded {
    [&](Literal &arg){
    },
    [&](VarIdent &arg){
      if(!hasVar(arg)) raiseError(node);
    },
    [&](ExprAstNode &arg){
      visit(arg);
    }
  }, node.operand);
}
void SemanticAnalyzer::visit(StatementsAstNode &node){
  for(auto &child : node.statements){
    visit(child);
  }
}

void SemanticAnalyzer::visit(AstNode &node){
  std::visit([&](auto &node){
    visit(node);
  }, node);
}

void SemanticAnalyzer::visit(IfAstNode &node){
  visit(node.expr);
  visit(node.ifStatements);
  visit(node.elseStatements);
}
void SemanticAnalyzer::visit(DeclAstNode &node){
  // initialize variable
  if(hasVar(node.varId)) raiseError(node); // redeclaration

  initVar(node.varId);
  if(auto &expr = node.value){
    visit(*expr);
  }
}
void SemanticAnalyzer::visit(AssignAstNode &node){
  if(!hasVar(node.varId)) raiseError(node);
  visit(node.expr);
}

void SemanticAnalyzer::visit(ForAstNode & node){
  if(!hasVar(node.varId)) raiseError(node);
  visit(node.startExpr);
  visit(node.endExpr);
  visit(node.statements);
}
void SemanticAnalyzer::visit(PrintAstNode & node){
  visit(node.expr);
}
void SemanticAnalyzer::visit(ReadAstNode & node){
  if(!hasVar(node.varId)) raiseError(node);
} 

void SemanticAnalyzer::initVar(std::string varId){
  variables.insert(varId);
}

bool SemanticAnalyzer::hasVar(VarIdent varId){
  return variables.count(varId);
}