#include <variant>
#include <type_traits>
#include "Parser.h"

ScannerIterator::ScannerIterator(Scanner &scanner) : scanner(scanner) {}

Token ScannerIterator::nextToken(){
  token = scanner.getToken();
  return token;
}
Token ScannerIterator::currentToken(){
  return token;
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
          // node = declaration();
          break;
        case Keyword::For:
          // node = forStatement();
          break;
        case Keyword::If:
          // node = ifStatement();
          break;
        case Keyword::Print:
          // node = printStatement();
          break;
        case Keyword::Read:
          // node = readStatement();
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
  
  return node;
}

AstNode Parser::assignment(){
  AssignAstNode node;
  node.varId = matchIdent(node);
  match(Delimiter::Assign, node);
  node.expr = expression();

  return node;
}

ExprAstNode Parser::expression(){
  ExprAstNode node;
  Token token = it.currentToken();
  node.opnd1 = std::make_unique<OpndAstNode>(operand());
  return node;
}
OpndAstNode Parser::operand(){
  OpndAstNode node;
  Token token = it.currentToken();
  std::visit( overloaded {
    [&](VarIdent &arg){
      // VarIdent
      node.operand = matchIdent(node);
    },
    [&](Literal &arg){
      // Literal
      node.operand = matchLiteral(node);
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

template<class NodeType>
Literal Parser::matchLiteral(NodeType &node){
  Token token = it.currentToken();
  if(std::holds_alternative<Literal>(token.value)){
    addMeta(node, token);
    it.nextToken();
    return std::get<Literal>(token.value);
  }else{
    // Error
    // return it.currentToken();
    throw std::exception();

    // exit(1);
  }
}

template<class NodeType>
VarIdent Parser::matchIdent(NodeType &node){
  Token token = it.currentToken();
  if(std::holds_alternative<VarIdent>(token.value)){
    addMeta(node, token);
    it.nextToken();
    return std::get<VarIdent>(token.value);
  }else{
    // Error
    // return it.currentToken();
    throw std::exception();
    // exit(1);
  }
}