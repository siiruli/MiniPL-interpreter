#include <iostream>
#include <assert.h>
#include "TypeChecker.h"


Type TypeChecker::visit(ExprNode &node){
  Type value;
  value = std::visit(overloaded {
    [&](LiteralNode &node) -> Type {
      return (std::holds_alternative<int>(node.literal.value)) 
                ? Type::Int : Type::String;;
      
    },
    [&](auto &node) -> Type {
      return visit(node);
    },
  }, node);
  return value;
}

Type TypeChecker::visit(UnaryOp &node){
  assert(node.opnd);
  Type type1 = visit(*node.opnd);
  Span span = getSpan(*node.opnd);
  Type type = Type::Broken;
  switch (node.op)
  {
    case Operator::Identity:
      type = type1; 
      break;
    case Operator::Not: 
      type = match(Bool, type1, node, span);
      break;
    default:
      break;
  }
  return type;
}


Type TypeChecker::visit(VarNode &node){
  return getType(node.varId);
}



Type TypeChecker::visit(BinaryOp &node){
  assert(node.opnd1);
  assert(node.opnd2);
  Type type1 = visit(*node.opnd1);
  Type type2 = visit(*node.opnd2);

  Span span1 = getSpan(*node.opnd1);
  Span span2 = getSpan(*node.opnd2);

  Type type = Type::Broken;
  switch (node.op)
  {
    case Operator::And:
      type = match(Bool, type1, node, span1);
      type = match(type1, type2, node, span2);      
      break;
    case Operator::Sub: 
    case Operator::Mul: 
    case Operator::Div: 
      type = match(Int, type1, node, span1);      
    case Operator::Add: 
      if(type1 == Type::Bool) {
        raiseError(node, {Int, String}, {type1}, span1);
        break;
      }
      type = match(type1, type2, node, span2);      
      break;
    case Operator::Equal: 
    case Operator::Less:
      type = match(type1, type2, node, span2);      
      if(type != Broken) type = Type::Bool;
      break;
    default:
      break;
  }
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
  type = match(Bool, type, node, getSpan(node.expr));
  visit(node.ifStatements);
  visit(node.elseStatements);
  return Type::Void;
}
Type TypeChecker::visit(DeclAstNode &node){
  // initialize variable
  if(auto &expr = node.value){
    Type exprtype = visit(*expr);
    match(node.type, exprtype, node, getSpan(*expr));
  }
  setType(node.varId, node.type);
  return Type::Void;
}
Type TypeChecker::visit(AssignAstNode &node){
  Type exprtype = visit(node.expr);
  match(getType(node.varId), exprtype, node, getSpan(node.expr));
  return Type::Void;
}

Type TypeChecker::visit(ForAstNode & node){
  match(Int, getType(node.varId), node, node.span);

  Type start = visit(node.startExpr);
  Type end = visit(node.endExpr);
  match(Int, start, node, getSpan(node.startExpr));
  match(Int, end, node, getSpan(node.endExpr));

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


void TypeChecker::setType(std::string varId, Type type){
  // variables[varId] = val;
  if(variables.count(varId)) return;
  else {
    variables[varId] = type;
  }
}
Type TypeChecker::getType(std::string varId){
  if(!variables.count(varId)) return Broken;
  else return variables[varId];
}

template<class NodeType> 
Type TypeChecker::match(Type type1, Type type2, NodeType &node, Span span){
  if(type1 == Broken || type2 == Broken) return Broken;
  else if(type1 != type2){
    raiseError(node, {type1}, {type2}, span);
    return Broken;
  }
  else return type1;
}

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