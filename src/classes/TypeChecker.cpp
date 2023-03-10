#include <iostream>
#include <assert.h>
#include "TypeChecker.h"


template<class NodeType> 
void TypeChecker::raiseError(NodeType &node, 
    Types exp, Types got){
  TypeError error;
  error.expected = exp;
  error.got = got;
  error.span = node.span;
  handler.raiseError(error);
}

Type TypeChecker::visit(ExprAstNode &node){
  Type type1, type2 = Type::Broken;
  type1 = visit(*node.opnd1);
  if(type1 == Type::Broken) return Type::Broken;

  if(node.opnd2) {
    type2 = visit(*node.opnd2);
    if(type2 == Type::Broken) return Type::Broken;
  }

  Type type = Type::Broken;
  switch (node.op)
  {
    case Operator::Identity:
      type = type1; 
      break;
    case Operator::Not: 
      if(type1 == Type::Bool) type = type1;
      else {
        raiseError(node, Types{Type::Bool}, Types{type1});
      }
      break;
    case Operator::And:
      if(type1 == Type::Bool && type2 == Type::Bool){
        type = Type::Bool;
      }else{
        raiseError(node, {Bool}, {type1, type2});
        break;
      }
    case Operator::Sub: 
    case Operator::Mul: 
    case Operator::Div: 
      if(type1 != Type::Int) {
        raiseError(node, {Int}, {type1, type2});
        break;
      }
    case Operator::Add: 
      if(type1 == Type::Bool) {
        raiseError(node, {Int, String}, {type1, type2});
        break;
      }
      else if(type1 != type2){
        raiseError(node, {type1}, {type1, type2});
        break;
      }
      type = type1;
      break;
    case Operator::Equal: 
    case Operator::Less:
      if(type1 != type2){
        raiseError(node, {type1}, {type1, type2});
        break;
      }
      type = Type::Bool;
      break;
  }
  return type;
}

Type TypeChecker::visit(OpndAstNode &node){
  Type type;
  std::visit( overloaded {
    [&](Literal &arg){
      // node.value = Literal.
      type = (std::holds_alternative<int>(arg.value)) ? Type::Int : Type::String;
    },
    [&](VarIdent &arg){
      type = getVal(arg);
    },
    [&](ExprAstNode &arg){
      type = visit(arg);
    }
  }, node.operand);
  return type;  
}
Type TypeChecker::visit(StatementsAstNode &node){
  for(auto &child : node.statements){
    visit(child);
  }
  return Type::Void;
}

Type TypeChecker::visit(AstNode &node){
  return std::visit([&](auto &node){
    return visit(node);
  }, node);
}

Type TypeChecker::visit(IfAstNode &node){
  visit(node.expr);
  visit(node.ifStatements);
  visit(node.elseStatements);
  return Type::Void;
}
Type TypeChecker::visit(DeclAstNode &node){
  // initialize variable
  initVar(node.type, node.varId);
  if(auto &expr = node.value){
    Type exprtype = visit(*expr);
    if(exprtype != Type::Broken && node.type != exprtype){
      raiseError(node, {node.type}, {exprtype});
      std::cout << node.type << " " << exprtype << "\n";
    }
  }
  return Type::Void;
}
Type TypeChecker::visit(AssignAstNode &node){
  Type exprtype = visit(node.expr);
  if(exprtype != Type::Broken && getVal(node.varId) != exprtype){
    raiseError(node, {getVal(node.varId)}, {exprtype});
  }
  // setVar(node.varId, node.expr.value);
  return Type::Void;
}

Type TypeChecker::visit(ForAstNode & node){
  visit(node.startExpr);
  visit(node.endExpr);
  visit(node.statements);
  return Type::Void;
}
Type TypeChecker::visit(PrintAstNode & node){
  visit(node.expr);
  return Type::Void;
}
Type TypeChecker::visit(ReadAstNode & node){
  return Type::Void;
} 

void TypeChecker::initVar(Type type, std::string varId){
  variables[varId] = type;
}
void TypeChecker::setVar(std::string varId, Type type){
  // variables[varId] = val;
  if(getVar(varId) != type){

  }
}
Type &TypeChecker::getVar(std::string varId){
  return variables[varId];
}
Type TypeChecker::getVal(std::string varId){
  return variables[varId];
}
