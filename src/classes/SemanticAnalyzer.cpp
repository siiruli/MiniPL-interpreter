#include <iostream>
#include <assert.h>
#include "SemanticAnalyzer.h"




void SemanticAnalyzer::visit(ExprNode &node){
  std::visit(overloaded {
    [&](LiteralNode &node) {
    },
    [&](VarNode &node) {
      accessVar(node, node.varId);
    },
    [&](UnaryOp &node) {
      visit(*node.opnd);
    },
    [&](BinaryOp &node) {
      visit(*node.opnd1);
      visit(*node.opnd2);
    },
  }, node);
}
void SemanticAnalyzer::visit(StatementsAstNode &node){
  for(auto &child : node.statements){
    visit(child);
  }
}

void SemanticAnalyzer::visit(StatementNode &node){
  std::visit([&](auto &node){
    visit(node);
  }, node);
}

void SemanticAnalyzer::visit(IfAstNode &node){
  visit(node.expr);
  scopes.push_back(node.span);
  visit(node.ifStatements);
  visit(node.elseStatements);
  scopes.pop_back();
}
void SemanticAnalyzer::visit(DeclAstNode &node){

  if(auto &expr = node.value){
    visit(*expr);
  }
  declareVar(node, node.varId);
}
void SemanticAnalyzer::visit(AssignAstNode &node){
  assignVar(node, node.varId);
  visit(node.expr);
}

void SemanticAnalyzer::visit(ForAstNode & node){
  assignVar(node.var, node.var.varId);
  visit(node.startExpr);
  visit(node.endExpr);
  
  scopes.push_back(node.span);
  getVar(node.var.varId).forParent = &node;
  visit(node.statements);
  getVar(node.var.varId).forParent = nullptr;
  scopes.pop_back();
}
void SemanticAnalyzer::visit(PrintAstNode & node){
  visit(node.expr);
}
void SemanticAnalyzer::visit(ReadAstNode & node){
  assignVar(node, node.varId);
} 

void SemanticAnalyzer::initVar(std::string varId){
  variables[varId] = variable{};
}

bool SemanticAnalyzer::hasVar(VarIdent varId){
  return variables.count(varId);
}
variable & SemanticAnalyzer::getVar(VarIdent varId){
  return variables[varId];
}

template<class NodeType> 
void SemanticAnalyzer::declareVar(NodeType &node, VarIdent varId){
  // initialize variable
  if(hasVar(node.varId)) {
    raiseError(node, node.span, varId, SemErrorType::ReDeclared);
  }
  if(!scopes.empty()){
    SemanticError error;
    error.identifier = varId;
    error.type = SemErrorType::WrongScope;
    error.contextSpan = scopes[0];
    error.context = "if of for";
    error.span = node.span;
    handler.raiseError(error);
  }
  initVar(node.varId);
}
template<class NodeType> 
void SemanticAnalyzer::assignVar(NodeType &node, VarIdent varId){
  if(!hasVar(varId)) {
    raiseError(node, node.span, node.varId, SemErrorType::NotDeclared);
  }
  else{
    ForAstNode *forParent = getVar(varId).forParent;
    if(forParent){
      raiseError(*forParent, node.span, varId, 
        SemErrorType::AssignConstant);
    }
  }

}
template<class NodeType> 
void SemanticAnalyzer::accessVar(NodeType &node, VarIdent varId){
  if(!hasVar(varId)) 
    raiseError(node, node.span, varId, SemErrorType::NotDeclared);
}


template<class NodeType> 
void SemanticAnalyzer::raiseError(NodeType &node, Span span, VarIdent varId, SemErrorType type){
  SemanticError error = createError<SemanticError>(span, node);
  error.identifier = varId;
  error.type = type;
  handler.raiseError(error);
}


