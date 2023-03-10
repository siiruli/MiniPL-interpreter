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
  visit(*node.opnd1);
  
  if(node.opnd2) {
    visit(*node.opnd2);
  }

  switch (node.op)
  {
    case Operator::Identity:
    case Operator::Not: 
    case Operator::And:
    case Operator::Sub: 
    case Operator::Mul: 
    case Operator::Div: 
    case Operator::Add: 
    case Operator::Equal: 
    case Operator::Less:
      break;
  }
}

void SemanticAnalyzer::visit(OpndAstNode &node){
  std::visit( overloaded {
    [&](Literal &arg){
      // node.value = Literal.
    },
    [&](VarIdent &arg){
    },
    [&](ExprAstNode &arg){
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
  initVar(node.varId);
  if(auto &expr = node.value){
    visit(*expr);
  }
}
void SemanticAnalyzer::visit(AssignAstNode &node){
  visit(node.expr);
}

void SemanticAnalyzer::visit(ForAstNode & node){
  visit(node.startExpr);
  visit(node.endExpr);
  visit(node.statements);
}
void SemanticAnalyzer::visit(PrintAstNode & node){
  visit(node.expr);
}
void SemanticAnalyzer::visit(ReadAstNode & node){
} 

void SemanticAnalyzer::initVar(std::string varId){
  variables.insert(varId);
}

bool SemanticAnalyzer::hasVar(VarIdent varId){
  return variables.count(varId);
}