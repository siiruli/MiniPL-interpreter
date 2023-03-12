#include <iostream>
#include <assert.h>
#include "InterpreterVisitor.h"



void InterpreterVisitor::visit(ExprAstNode &node){
  visit(*node.opnd1);
  ExprValue opnd1 = node.opnd1.get()->value;
  ExprValue opnd2 = 0;  
  if(node.opnd2) {
    visit(*node.opnd2);
    opnd2 = node.opnd2.get()->value;
  }
  ExprValue val;
  
  switch (node.op)
  {
    case Operator::Identity: val = opnd1; break;
    case Operator::Not: val = Op::logNot(opnd1); break;
    case Operator::And: val = funcVisitor(Op::logAnd, opnd1, opnd2); break;
    case Operator::Equal: val = funcVisitor(Op::eq, opnd1, opnd2); break;
    case Operator::Less: val = funcVisitor(Op::less, opnd1, opnd2); break;
    case Operator::Add: val = funcVisitor(Op::add, opnd1, opnd2); break;
    case Operator::Sub: val = funcVisitor(Op::sub, opnd1, opnd2); break;
    case Operator::Mul: val = funcVisitor(Op::mul, opnd1, opnd2); break;
    case Operator::Div: 
      val = funcVisitor(Op::div, opnd1, opnd2); 
      break;
  }
  node.value = val;
}




void InterpreterVisitor::visit(OpndAstNode &node){

  std::visit( overloaded {
    [&](Literal &arg){
      // node.value = Literal.
      std::visit([&](auto &arg){
        node.value = arg;
      }, arg.value);
    },
    [&](VarIdent &arg){
      node.value = getVal(arg);
    },
    [&](ExprAstNode &arg){
      visit(arg);
      node.value = arg.value;
    }
  }, node.operand);
  
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
  if(auto &expr = node.value){
    visit(*expr);
    setVar(node.varId, (*expr).value);
  }
}
void InterpreterVisitor::visit(AssignAstNode &node){
  visit(node.expr);
  setVar(node.varId, node.expr.value);
}

void InterpreterVisitor::visit(ForAstNode & node){
  visit(node.startExpr);
  visit(node.endExpr);
  assert(std::holds_alternative<int>(node.startExpr.value));
  int start = std::get<int>(node.startExpr.value);
  assert(std::holds_alternative<int>(node.endExpr.value));
  int end = std::get<int>(node.endExpr.value);
  for(int i= start; i<= end; ++i){
    this->setVar(node.varId, i);
    visit(node.statements);
  }
}
void InterpreterVisitor::visit(PrintAstNode & node){
  visit(node.expr);
  std::visit([&](auto& value) {
    output << value;
  }, node.expr.value);
}
void InterpreterVisitor::visit(ReadAstNode & node){
  std::visit([&](auto &value) {
    input >> value;
  }, getVar(node.varId));
} 

void InterpreterVisitor::initVar(Type type, std::string varId){
  ExprValue val;
  switch (type)  {
    case Int: val = int(0); break;
    case Bool: val = bool(false); break;
    case String: val = ""; break;
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