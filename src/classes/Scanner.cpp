#include "Scanner.h"


#include <iostream>
#include <vector>

Scanner::Scanner(std::string &programText) : program(programText) {
} 

Token Scanner::getToken(){
  auto token = scanToken();
  while(!token.has_value()) token = scanToken();
  return token.value();
}

std::optional<Token> Scanner::scanToken(){
  // Skip whitespace and comments
  while(auto c = program.currentChar()){
    // skip over comments
    if(*c == '/'){
      auto next = program.peekChar();
      if(next == '/' || next == '*') scanComment();
    
    }
    // end if c is not whitespace
    else if(!isspace(static_cast<unsigned char>(*c))) break;

    program.move();
  }

  // scan a token  
  Position startPos = program.currentPosition();
  std::optional<std::pair<TokenType, TokenValue>> token = std::nullopt;

  if(auto curChar = program.currentChar()){
    unsigned char c = static_cast<unsigned char>(*curChar);
    if(isalpha(c)){
      std::string id = scanIdentifier();
      // if id is a keyword, return the keyword
      if(auto keyword = isKeyword(id)){
        token  = {TokenType::Keyword, *keyword};
      }else{
        token = {TokenType::Identifier, id};
      }
    }
    if(isdigit(c)) token = {TokenType::Literal, scanInteger()};
    if(c == '"') token = {TokenType::Literal, scanString()};
  }
  else{
    token = {TokenType::Punctuation, Punctuation::Eof};
  }

  Position endPos = program.currentPosition();
  program.move();

  if(token.has_value()) {
    auto [type, value] = token.value();
    return Token{type, startPos, endPos, value};
  }else {
    return {};
  }
}

void Scanner::scanComment(){
  char c1 = program.currentChar().value();
  program.move();
  char c2 = program.currentChar().value();
  program.move();

  if(c1 == '/' && c2 == '/'){
    while(program.currentChar() != '\n'){
      program.move();
    }
    program.move();
  }else if(c1 =='/' && c2 == '*'){
    int nestingLevel = 1;
    while(auto c = program.currentChar()){
      if(nestingLevel == 0) break;
      if(*c == '/' && program.peekChar() == '*') {
        ++nestingLevel;
        program.move();
      }
      else if(*c == '*' && program.peekChar() == '/'){
        --nestingLevel;
        program.move();
      }
      program.move();
    }
  }
}
std::optional<Keyword> Scanner::isKeyword(std::string &ident){
  for(int i = 0; i<n_keywords; ++i) {
    if(ident == keywords[i]) 
      return static_cast<Keyword>(i);
  }
  return {};
}

std::string Scanner::scanIdentifier(){
  std::string ident = "";
  ident += program.currentChar().value();
  while(auto c = program.peekChar()){
    if(!isalnum(static_cast<unsigned char>(*c))) break;
    ident += *c;
    program.move();
  }
  return ident;
}
int Scanner::scanInteger(){
  std::string lexeme = "";
  lexeme += program.currentChar().value();
  while(auto c = program.peekChar()){
    if(!isdigit(static_cast<unsigned char>(*c))) break;
    lexeme += *c;
    program.move();
  }
  return std::stoi(lexeme);
}

std::string Scanner::scanString(){
  std::string lexeme = "";
  std::string error = "";

  if( auto c = program.currentChar(); *c == '"'){
    program.move();
  }

  while(program.currentChar() != '"'){
    if(!program.currentChar().has_value()){
      // error
    }
    char c = program.currentChar().value();
    if(c == '\n'){
      // ERROR
    }
    
    if(c == '\\'){
      // escape characters
      if(auto c = program.peekChar()){
        lexeme += *c;
      }else{
        // ERRROR
      }
    }else{
      lexeme += c;
    }

    program.move();
  }
  return lexeme;
}

