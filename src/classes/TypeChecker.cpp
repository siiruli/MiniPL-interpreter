// #include <iostream>
// #include <assert.h>
// #include "TypeChecker.h"




// Type TypeChecker::visit(ExprAstNode &node){
//   Type type1, type2 = Type::Broken;
//   type1 = visit(*node.opnd1);
  
//   if(type1 == Type::Broken) return Type::Broken;

//   if(node.opnd2) {
//     type2 = visit(*node.opnd2);
//     if(type2 == Type::Broken) return Type::Broken;
//   }

//   Type type = Type::Broken;
//   switch (node.op)
//   {
//     case Operator::Identity:
//       type = type1; 
//       break;
//     case Operator::Not: 
//       type = match(Bool, type1, node, node.opnd1->span);
//       break;
//     case Operator::And:
//       type = match(Bool, type1, node, node.opnd1->span);
//       type = match(type1, type2, node, node.opnd2->span);      
//       break;
//     case Operator::Sub: 
//     case Operator::Mul: 
//     case Operator::Div: 
//       type = match(Int, type1, node, node.opnd1->span);      
//     case Operator::Add: 
//       if(type1 == Type::Bool) {
//         raiseError(node, {Int, String}, {type1}, node.opnd1->span);
//         break;
//       }
//       type = match(type1, type2, node, node.opnd2->span);      
//       break;
//     case Operator::Equal: 
//     case Operator::Less:
//       type = match(type1, type2, node, node.opnd2->span);      
//       if(type != Broken) type = Type::Bool;
//       break;
//   }
//   return type;
// }

// Type TypeChecker::visit(OpndAstNode &node){
//   Type type;
//   std::visit( overloaded {
//     [&](Literal &arg){
//       // node.value = Literal.
//       type = (std::holds_alternative<int>(arg.value)) ? Type::Int : Type::String;
//     },
//     [&](VarIdent &arg){
//       type = getType(arg);
//     },
//     [&](ExprAstNode &arg){
//       type = visit(arg);
//     }
//   }, node.operand);
//   return type;  
// }
// Type TypeChecker::visit(StatementsAstNode &node){
//   for(auto &child : node.statements){
//     visit(child);
//   }
//   return Type::Void;
// }

// Type TypeChecker::visit(AstNode &node){
//   return std::visit([&](auto &node){
//     return visit(node);
//   }, node);
// }

// Type TypeChecker::visit(IfAstNode &node){
//   Type type = visit(node.expr);
//   type = match(Bool, type, node, node.expr.span);
//   visit(node.ifStatements);
//   visit(node.elseStatements);
//   return Type::Void;
// }
// Type TypeChecker::visit(DeclAstNode &node){
//   // initialize variable
//   if(auto &expr = node.value){
//     Type exprtype = visit(*expr);
//     match(node.type, exprtype, node, (*expr).span);
//   }
//   setType(node.varId, node.type);
//   return Type::Void;
// }
// Type TypeChecker::visit(AssignAstNode &node){
//   Type exprtype = visit(node.expr);
//   match(getType(node.varId), exprtype, node, node.expr.span);
//   return Type::Void;
// }

// Type TypeChecker::visit(ForAstNode & node){
//   match(Int, getType(node.varId), node, node.span);

//   Type start = visit(node.startExpr);
//   Type end = visit(node.endExpr);
//   match(Int, start, node, node.startExpr.span);
//   match(Int, end, node, node.endExpr.span);

//   visit(node.statements);
//   return Type::Void;
// }
// Type TypeChecker::visit(PrintAstNode & node){
//   visit(node.expr);
//   return Type::Void;
// }
// Type TypeChecker::visit(ReadAstNode & node){
//   return Type::Void;
// } 


// void TypeChecker::setType(std::string varId, Type type){
//   // variables[varId] = val;
//   if(variables.count(varId)) return;
//   else {
//     variables[varId] = type;
//   }
// }
// Type TypeChecker::getType(std::string varId){
//   if(!variables.count(varId)) return Broken;
//   else return variables[varId];
// }

// template<class NodeType> 
// Type TypeChecker::match(Type type1, Type type2, NodeType &node, Span span){
//   if(type1 == Broken || type2 == Broken) return Broken;
//   else if(type1 != type2){
//     raiseError(node, {type1}, {type2}, span);
//     return Broken;
//   }
//   else return type1;
// }

// template<class NodeType> 
// void TypeChecker::raiseError(NodeType &node, 
//     Types exp, Types got, Span span){
  
//   TypeError error;
  
//   error.span = span;
//   error.expected = exp;
//   error.got = got;
//   raiseError(node, error);
// }


// template<class NodeType> 
// void TypeChecker::raiseError(NodeType &node, TypeError error){
//   error.contextSpan = node.span;
//   error.context = astNodeName<NodeType>();
//   handler.raiseError(error);
// }