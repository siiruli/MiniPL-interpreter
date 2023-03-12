#include <iostream>
#include <assert.h>
#include "InterpreterVisitor.h"

ExprValue InterpreterVisitor::visit(ExprNode &node){
  ExprValue value;
  value = std::visit(overloaded {
    [&](LiteralNode &node) -> ExprValue {
      return std::visit([&](auto &arg){
        return ExprValue{arg};
      }, node.literal.value);
    },
    [&](auto &node) -> ExprValue {
      return visit(node);
    },
  }, node);
  return value;
}

ExprValue InterpreterVisitor::visit(UnaryOp &node){
  assert(node.opnd);
  ExprValue opnd = visit(*node.opnd);
  ExprValue val;
  switch (node.op)
  {
    case Operator::Identity: val = opnd; break;
    case Operator::Not: val = Op::logNot(opnd); break;
    default:
      throw RunTimeException("Wrong operand");
      break;
  }
  return val;
}
ExprValue InterpreterVisitor::visit(BinaryOp &node){
  assert(node.opnd1);
  assert(node.opnd2);
  ExprValue opnd1 = visit(*node.opnd1);
  ExprValue opnd2 = visit(*node.opnd2);
  ExprValue val;
  try {
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
  } catch (RunTimeException &e){
    RuntimeError error;
    error.span = node.span;
    error.data = e.what();
    error.addContext(node);
    handler.raiseError(error);
    throw RunTimeException(e.what());
  }
  return val;
}

ExprValue InterpreterVisitor::visit(VarNode &node){
  return getVal(node.varId);
}


void InterpreterVisitor::visit(StatementsAstNode &node){
  for(auto &child : node.statements){
    visit(child);
  }
}

void InterpreterVisitor::visit(StatementNode &node){
  std::visit([&](auto &node){
    visit(node);
  }, node);
}

void InterpreterVisitor::visit(IfAstNode &node){
  ExprValue val = visit(node.expr);
  if(val == ExprValue{true}){
    visit(node.ifStatements);
  }else{
    visit(node.elseStatements);
  }
}
void InterpreterVisitor::visit(DeclAstNode &node){
  // initialize variable
  initVar(node.type, node.varId);
  if(auto &expr = node.value){
    auto val = visit(*expr);
    setVar(node.varId, val);
  }
}
void InterpreterVisitor::visit(AssignAstNode &node){
  ExprValue val = visit(node.expr);
  setVar(node.varId, val);
}

void InterpreterVisitor::visit(ForAstNode & node){
  ExprValue startVal = visit(node.startExpr);
  ExprValue endVal = visit(node.endExpr);
  assert(std::holds_alternative<int>(startVal));
  int start = std::get<int>(startVal);
  assert(std::holds_alternative<int>(endVal));
  int end = std::get<int>(endVal);

  for(int i= start; i<= end; ++i){
    this->setVar(node.var.varId, i);
    visit(node.statements);
  }
}
void InterpreterVisitor::visit(PrintAstNode & node){
  ExprValue val = visit(node.expr);
  std::visit([&](auto& value) {
    output << value;
  }, val);
}
void InterpreterVisitor::visit(ReadAstNode & node){
  try{
    std::visit([&](auto &value) {
      input >> value;
    }, getVar(node.varId));
  } catch (std::ios_base::failure &e){
    RuntimeError error;
    error.data = "Could not read " + node.varId + " from input";
    error.addContext(node);
    error.span = node.span;
    handler.raiseError(error);
    throw RunTimeException("IO failure");
  }
} 

void InterpreterVisitor::initVar(Type type, std::string varId){
  ExprValue val;
  switch (type)  {
    case Int: val = int(0); break;
    case Bool: val = bool(false); break;
    case String: val = ""; break;
    default: break;
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