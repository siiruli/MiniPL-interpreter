#include <iostream>
#include <assert.h>
#include "Interpreter.h"
// operator implementations
namespace Op {
  auto add = overloaded {
    [](auto a, auto b){return a+b;},
    [](bool a, bool b){throw(RunTimeException("wrong types")); return "TypeError";}
  };
  auto sub = overloaded {
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";},
    [](num a, num b){return a-b;}
  };
  auto mul = overloaded {
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";},
    [](num a, num b){return a*b;}
  };
  auto div = overloaded {
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";},
    [](num a, num b){
      if(b == 0){
        throw RunTimeException("Division by zero");
      }
      return a/b;
    }
  };
  auto eq = [](auto a, auto b){return a == b;};
  auto less = [](auto a, auto b){return a < b;};  
  auto logAnd = overloaded {
    [](bool a, bool b){return a && b;},
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";}
  };
  auto logNot = [](ExprValue val) {
    if(std::holds_alternative<bool>(val)){
      return !std::get<bool>(val);
    }else{
      throw(RunTimeException("wrong type"));
    }
  };

}

ExprValue Interpreter::visit(ExprNode &node){
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

ExprValue Interpreter::visit(UnaryOp &node){
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
ExprValue Interpreter::visit(BinaryOp &node){
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
    RuntimeError error = createError<RuntimeError>(node.span, node);
    error.data = e.what();
    handler.raiseError(error);
    throw RunTimeException(e.what());
  }
  return val;
}

ExprValue Interpreter::visit(VarNode &node){
  return getVal(node.varId);
}


void Interpreter::visit(StatementsAstNode &node){
  for(auto &child : node.statements){
    visit(child);
  }
}

void Interpreter::visit(StatementNode &node){
  std::visit([&](auto &node){
    visit(node);
  }, node);
}

void Interpreter::visit(IfAstNode &node){
  ExprValue val = visit(node.expr);
  if(val == ExprValue{true}){
    visit(node.ifStatements);
  }else{
    visit(node.elseStatements);
  }
}
void Interpreter::visit(DeclAstNode &node){
  // initialize variable
  initVar(node.type, node.varId);
  if(auto &expr = node.value){
    auto val = visit(*expr);
    setVar(node.varId, val);
  }
}
void Interpreter::visit(AssignAstNode &node){
  ExprValue val = visit(node.expr);
  setVar(node.varId, val);
}

void Interpreter::visit(ForAstNode & node){
  ExprValue startVal = visit(node.startExpr);
  ExprValue endVal = visit(node.endExpr);
  assert(std::holds_alternative<num>(startVal));
  num start = std::get<num>(startVal);
  assert(std::holds_alternative<num>(endVal));
  num end = std::get<num>(endVal);

  for(num i= start; i<= end; ++i){
    this->setVar(node.var.varId, i);
    visit(node.statements);
  }
}
void Interpreter::visit(PrintAstNode & node){
  ExprValue val = visit(node.expr);
  std::visit([&](auto& value) {
    output << value;
  }, val);
}
void Interpreter::visit(ReadAstNode & node){
  try{
    std::visit([&](auto &value) {
      input >> value;
    }, getVar(node.varId));
  } catch (std::ios_base::failure &e){
    RuntimeError error = createError<RuntimeError>(node.span, node);
    error.data = "Could not read " + node.varId + " from input";
    handler.raiseError(error);
    throw RunTimeException("IO failure");
  }
} 

void Interpreter::initVar(Type type, std::string varId){
  ExprValue val;
  switch (type)  {
    case Int: val = num{0}; break;
    case Bool: val = bool(false); break;
    case String: val = ""; break;
    default: break;
  }
  variables[varId] = val;
}
void Interpreter::setVar(std::string varId, ExprValue val){
  variables[varId] = val;
}
ExprValue &Interpreter::getVar(std::string varId){
  return variables[varId];
}
ExprValue Interpreter::getVal(std::string varId){
  return variables[varId];
}