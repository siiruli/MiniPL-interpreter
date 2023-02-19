#include <iostream>
#include "InterpreterVisitor.h"




void InterpreterVisitor::visitIf(IfAstNode &node){
  node.expr.visit(this);
  if(node.expr.value){
    node.ifStatements.visit(this);
  }else{
    node.elseStatements.visit(this);
  }
}
void InterpreterVisitor::visitDecl(DeclAstNode &node){
  this->init(node.type, node.varId);
}
void InterpreterVisitor::visitAssign(AssignAstNode &node){
  node.expr.visit(this);
  this->setValue(node.varId, node.expr.value);
}

void InterpreterVisitor::visitFor(ForAstNode & node){
  node.startExpr.visit(this);
  node.endExpr.visit(this);
  for(int i=node.startExpr.value; i<= node.endExpr.value; ++i){
    this->setValue(node.varId, i);
    node.statements.visit(this);
  }
}
void InterpreterVisitor::visitPrint(PrintAstNode & node){
  node.expr.visit(this);
  std::cout << node.expr.value << std::endl;
}
void InterpreterVisitor::visitRead(ReadAstNode & node){
  switch (this->getType(node.varId)){
    case Int:
      int value;
      std::cin >> value;
      this->setValue(node.varId, value);
      break;
    case String:
      std::string value;
      std::cin >> value;
      this->setValue(node.varId, value);
  }
} 
