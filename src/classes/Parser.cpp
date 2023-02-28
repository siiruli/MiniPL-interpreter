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
    node.statements.push_back(*stmt);  
    addMeta(node, stmt.value());
  }
  return node;
}

std::optional<AstNode> Parser::statement(){
  std::optional<AstNode> node; 
  Token token = it.currentToken();
  std::visit( overloaded {
    [&](std::string &arg){
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
  VarIdent var = matchIdent(node);
  match(Delimiter::Assign, node);
  AstNode expr; // = expression();

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
  if(it.currentToken().value == expected){
    addMeta(node, it.currentToken());
    it.nextToken();
  }else{
    // ERROR
    exit(1);
  }
}

template<class NodeType>
Literal Parser::matchLiteral(NodeType &node){
  if(std::holds_alternative<Literal>(it.currentToken().value)){
    Token token = it.currentToken();
    addMeta(node, it.currentToken());
    it.nextToken();
    return std::get<Literal>(token.value);
  }else{
    // Error
    // return it.currentToken();
    exit(1);
  }
}

template<class NodeType>
VarIdent Parser::matchIdent(NodeType &node){
  if(std::holds_alternative<VarIdent>(it.currentToken().value)){
    Token token = it.currentToken();
    addMeta(node, token);
    it.nextToken();
    return std::get<VarIdent>(token.value);
  }else{
    // Error
    // return it.currentToken();
    exit(1);
  }
}