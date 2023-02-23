#include <variant>
#include <type_traits>
#include "Parser.h"
Parser::Parser(Scanner &scanner) : scanner(scanner)
{
};

void Parser::nextToken(){
  currentToken = scanner.getToken();
}
AstNode Parser::program(){
  nextToken();
  return statements();
}

StatementsAstNode Parser::statements(){
  std::vector<AstNode> stmts;
  while(
    currentToken.value != static_cast<TokenValue>(Punctuation::Eof)
  ){
    stmts.push_back(statement());  
  }
  StatementsAstNode node;
  node.statements = stmts;
  return node;
}

AstNode Parser::statement(){
  AstNode node; 
  std::visit( overloaded {
    [&](std::string &arg){
      // node = assignment();
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
          node = ErrorAstNode();
      }
    },
    [&](auto &arg){
      node = ErrorAstNode();
      // ERROR
    }
  }, currentToken.value);

  return node;
}


void Parser::match(TokenValue expected){
  if(currentToken.value == expected){
    nextToken();
  }else{
    // ERROR
  }
}