#include <iostream>
#include "InterpreterVisitor.h"

void InterpreterVisitor::visit(ExprAstNode &node){
  visit(node.opnd1.get());
  node.value = node.opnd1.get()->value;
}
void InterpreterVisitor::visit(OpndAstNode &node){

}

void InterpreterVisitor::visit(OpndAstNode *node){

  std::visit( overloaded {
    [&](Literal &arg){
      // node.value = Literal.
      std::visit([&](auto &arg){
        node->value = arg;
      }, arg.value);
    },
    [&](VarIdent &arg){
      node->value = 0;
    },
    [&](ExprAstNode &arg){
      visit(arg);
      node->value = arg.value;
    }
  }, node->operand);
  
}
void InterpreterVisitor::visit(StatementsAstNode &node){
  for(auto &child : node.statements){
    visit(child);
  }
}

void InterpreterVisitor::visit(AstNode &node){
  std::visit([&](auto &node){
    visit(node);
  }, node);
}

void InterpreterVisitor::visit(IfAstNode &node){
  visit(node.expr);
  if(std::get<bool>(node.expr.value) == true){
    visit(node.ifStatements);
  }else{
    visit(node.elseStatements);
  }
}
void InterpreterVisitor::visit(DeclAstNode &node){
  // initialize variable
  initVar(node.type, node.varId);
}
void InterpreterVisitor::visit(AssignAstNode &node){
  visit(node.expr);
  setVar(node.varId, node.expr.value);
}

void InterpreterVisitor::visit(ForAstNode & node){
  visit(node.startExpr);
  visit(node.endExpr);
  int start = std::get<int>(node.startExpr.value);
  int end = std::get<int>(node.endExpr.value);
  for(int i= start; i<= end; ++i){
    this->setVar(node.varId, i);
    visit(node.statements);
  }
}
void InterpreterVisitor::visit(PrintAstNode & node){
  visit(node.expr);
  std::visit([](auto& value) {
    std::cout << value << std::endl;
  }, node.expr.value);
}
void InterpreterVisitor::visit(ReadAstNode & node){
  std::visit([](auto &value) {
    std::cin >> value;
  }, getVar(node.varId));
} 

void InterpreterVisitor::initVar(Type type, std::string varId){
  ExprValue val;
  switch (type)  {
    case Int: val = 0;
    case Bool: val = false;
    case String: val = "";
  }
  variables[varId] = val;
}
void InterpreterVisitor::setVar(std::string varId, ExprValue val){
  variables[varId] = val;
}
ExprValue &InterpreterVisitor::getVar(std::string varId){
  return variables[varId];
}
ExprValue InterpreterVisitor::getVal(std::string varId){
  return variables[varId];
}
void InterpreterVisitor::visit(ErrorAstNode &node){

}