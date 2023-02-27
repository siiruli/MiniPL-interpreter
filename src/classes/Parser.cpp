#include <variant>
#include <type_traits>
#include "Parser.h"

TokenIterator::TokenIterator(Scanner &scanner) : scanner(scanner) {}

Token TokenIterator::nextToken(){
  token = scanner.getToken();
  return token;
}
Token TokenIterator::currentToken(){
  return token;
}

Parser::Parser(Scanner &scanner) : it(scanner) {};


AstNode Parser::program(){
  it.nextToken();
  AstNode stmts = statements();
  match(Delimiter::Eof);
  return stmts;
}

StatementsAstNode Parser::statements(){
  std::vector<AstNode> stmts;
  while(auto node = statement()){
    stmts.push_back(*node);  
  }
  StatementsAstNode node;
  node.statements = stmts;
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
  Token var = matchIdent();
  match(Delimiter::Assign);
  AstNode expr; // = expression();

  AssignAstNode node;
  return node;
}



void Parser::match(TokenValue expected){
  if(it.currentToken().value == expected){
    it.nextToken();
  }else{
    // ERROR
  }
}

Token Parser::matchLiteral(){
  if(std::holds_alternative<Literal>(it.currentToken().value)){
    Token token = it.currentToken();
    it.nextToken();
    return token;
  }else{
    // Error
    return it.currentToken();
  }
}

Token Parser::matchIdent(){
  if(std::holds_alternative<VarIdent>(it.currentToken().value)){
    Token token = it.currentToken();
    it.nextToken();
    return token;
  }else{
    // Error
    return it.currentToken();
  }
}