#include <variant>
#include <type_traits>
#include "Parser.h"

ScannerIterator::ScannerIterator(Scanner &scanner) 
  : scanner(scanner) 
{
}

Token ScannerIterator::nextToken(){
  token = scanner.getToken();
  return token.value();
}
Token ScannerIterator::currentToken(){
  if(!token.has_value()) token = scanner.getToken();
  return token.value();
}

Parser::Parser(TokenIterator &it) : it(it) {};


AstNode Parser::program(){
  StatementsAstNode stmts = statements();
  match(Delimiter::Eof, stmts);
  return stmts;
}

StatementsAstNode Parser::statements(){
  StatementsAstNode node;
  while(auto stmt = statement()){
    node.statements.push_back(std::move(*stmt));
    addMeta(node, stmt.value());
  }
  return node;
}

std::optional<AstNode> Parser::statement(){
  std::optional<AstNode> node; 
  Token token = it.currentToken();
  std::visit( overloaded {
    [&](VarIdent &arg){
      node = assignment();
    },
    [&](Keyword &arg){
      switch (arg)
      {
        case Keyword::Var:
          node = declaration();
          break;
        case Keyword::For:
          node = forStatement();
          break;
        case Keyword::If:
          node = ifStatement();
          break;
        case Keyword::Print:
          node = printStatement();
          break;
        case Keyword::Read:
          node = readStatement();
          break;
        default:
          // epsilon rule
          node = std::nullopt;
      }
    },
    [&](auto &arg){
      // epsilon rule
      node = std::nullopt;
    }
  }, token.value);
  if(node.has_value()){
    std::visit([&](auto &node){
      match(Delimiter::Semicolon, node);
    }, node.value());
  }

  return node;
}

AstNode Parser::assignment(){
  AssignAstNode node;
  node.varId = match<VarIdent>(node);
  match(Delimiter::Assign, node);
  node.expr = expression();

  return node;
}

AstNode Parser::declaration(){
  DeclAstNode node;
  match(Keyword::Var, node);
  node.varId = match<VarIdent>(node);
  match(Delimiter::Colon, node);
  Keyword kw = match<Keyword>(node);
  switch (kw)
  {
  case Keyword::Int: node.type = Type::Int; break;
  case Keyword::Bool: node.type = Type::Bool; break;
  case Keyword::String: node.type = Type::String; break;  
  default:
    // Error
    break;
  }
  if(it.currentToken().value == TokenValue{Delimiter::Assign}){
    match(Delimiter::Assign, node);
    node.value = expression();
  }
  return node;
}

AstNode Parser::forStatement(){
  ForAstNode node;
  match(Keyword::For, node);
  node.varId = match<VarIdent>(node);
  match(Keyword::In, node);
  node.startExpr = expression();
  match(Delimiter::Range, node);
  node.endExpr = expression();
  match(Keyword::Do, node);
  node.statements = statements();
  match(Keyword::End, node);
  match(Keyword::For, node);
  return node;
}
AstNode Parser::ifStatement(){
  IfAstNode node;
  match(Keyword::If, node);
  node.expr = expression();
  match(Keyword::Do, node);
  node.ifStatements = statements();
  if(it.currentToken().value == TokenValue{Keyword::Else}){
    match(Keyword::Else, node);
    match(Keyword::Do, node);
    node.elseStatements = statements();
  }
  match(Keyword::End, node);
  match(Keyword::If, node);

  return node;
}
AstNode Parser::readStatement(){
  ReadAstNode node;
  match(Keyword::Read, node);
  node.varId = match<VarIdent>(node);
  return node;
}
AstNode Parser::printStatement(){
  PrintAstNode node;
  match(Keyword::Print, node);
  node.expr = expression();
  return node;
}

ExprAstNode Parser::expression(){
  ExprAstNode node;
  Token token = it.currentToken();
  if(token.value == TokenValue{Operator::Not}){
    node.op = match<Operator>(node);
    node.opnd1 = std::make_unique<OpndAstNode>(operand());
  }else{
    
    node.opnd1 = std::make_unique<OpndAstNode>(operand());

    if(std::holds_alternative<Operator>(it.currentToken().value)){
      node.op = match<Operator>(node);
      node.opnd2 = std::make_unique<OpndAstNode>(operand());
    }
  }
  return node;
}
OpndAstNode Parser::operand(){
  OpndAstNode node;
  Token token = it.currentToken();
  std::visit( overloaded {
    [&](VarIdent &arg){
      // VarIdent
      node.operand = match<VarIdent>(node);
    },
    [&](Literal &arg){
      // Literal
      node.operand = match<Literal>(node);
    },
    [&](Delimiter &arg){
      // (
      match(Delimiter::OpenParen, node);
      node.operand = expression();
      match(Delimiter::ClosedParen, node);
    },
    [&](auto &arg){
      // epsilon rule
      throw std::exception();
    }
  }, token.value);
  return node;
}
// Add metadata from a token to an AST node
template<class NodeType>
void Parser::addMeta(NodeType &node, Token token){
  node.span += token.span;
  // AstNodeBase &base = getBaseReference(node);
  // base.span += token.span;
}
// Add metadata from a child AST node to its parent
template<class NodeType>
void Parser::addMeta(NodeType &node, AstNode &childNode){
  AstNodeBase &childBase = getBaseReference(childNode);
  node.span += childBase.span;
}
// Check that current token has the expected value, 
// and move to next token.
// Also add metadata from token to node.
template<class NodeType>
void Parser::match(TokenValue expected, NodeType &node){
  Token token = it.currentToken();
  if(token.value == expected){
    addMeta(node, it.currentToken());
    it.nextToken();
  }else{
    // ERROR
    throw std::exception();

    // exit(1);
  }
}

// Check thet current token has the expected type, 
// return the value, and move to next token.
// Also add metadata from token to node.
template<class ExpectedType, class NodeType>
ExpectedType Parser::match(NodeType &node){
  Token token = it.currentToken();
  if(std::holds_alternative<ExpectedType>(token.value)){
    addMeta(node, token);
    it.nextToken();
    return std::get<ExpectedType>(token.value);
  }else{
    // Error
    // return it.currentToken();
    throw std::exception();

    // exit(1);
  }
}
