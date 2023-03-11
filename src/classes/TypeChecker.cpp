#include <iostream>
#include <assert.h>
#include "TypeChecker.h"


template<class NodeType> 
void TypeChecker::raiseError(NodeType &node, 
    Types exp, Types got, Span span){
  
  TypeError error;
  
  error.span = span;
  error.expected = exp;
  error.got = got;
  raiseError(node, error);
}


template<class NodeType> 
void TypeChecker::raiseError(NodeType &node, TypeError error){
  error.contextSpan = node.span;
  error.context = astNodeName<NodeType>();
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
        raiseError(node, Types{Type::Bool}, Types{type1}, node.opnd1->span);
      }
      break;
    case Operator::And:
      if(type1 != Type::Bool){
        raiseError(node, {Bool}, {type1}, node.opnd1->span);
        break;
      }
      if(type2 != Type::Bool){
        raiseError(node, {Bool}, {type2}, node.opnd2->span);
        break;
      }
      type = Type::Bool;
      break;
    case Operator::Sub: 
    case Operator::Mul: 
    case Operator::Div: 
      if(type1 != Type::Int) {
        raiseError(node, {Int}, {type1}, node.opnd1->span);
        break;
      }
    case Operator::Add: 
      if(type1 == Type::Bool) {
        raiseError(node, {Int, String}, {type1}, node.opnd1->span);
        break;
      }
      
      if(type1 != type2){
        raiseError(node, {type1}, {type2}, node.opnd2->span);
        break;
      }
      type = type1;
      break;
    case Operator::Equal: 
    case Operator::Less:
      if(type1 != type2){
        raiseError(node, {type1}, {type2}, node.opnd2->span);
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
  Type type = visit(node.expr);
  if(type != Bool){
    raiseError(node, {Bool}, {type}, node.expr.span);
  }
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
      raiseError(node, {node.type}, {exprtype}, (*expr).span);
    }
  }
  return Type::Void;
}
Type TypeChecker::visit(AssignAstNode &node){
  Type exprtype = visit(node.expr);
  if(exprtype != Type::Broken && getVal(node.varId) != exprtype){
    raiseError(node, {getVal(node.varId)}, {exprtype}, node.expr.span);
  }
  // setVar(node.varId, node.expr.value);
  return Type::Void;
}

Type TypeChecker::visit(ForAstNode & node){
  if(getVal(node.varId) != Int){
    raiseError(node, {Int}, {getVal(node.varId)}, node.span);
  }
  Type start = visit(node.startExpr);
  Type end = visit(node.endExpr);
  if(start != Int){
    raiseError(node, {Int}, {start}, node.startExpr.span);
  }
  if(end != Int){
    raiseError(node, {Int}, {end}, node.endExpr.span);
  }

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
