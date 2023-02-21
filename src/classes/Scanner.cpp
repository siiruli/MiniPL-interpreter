#include "Scanner.h"


#include <iostream>
#include <vector>

Scanner::Scanner(std::string &programText) : program(programText) {
} 


std::optional<Token> Scanner::getToken(){
  // Skip whitespace and comments
  while(auto c = program.currentChar()){
    // skip over comments
    if(*c == '/'){
      auto next = program.peekChar();
      if(next == '/' || next == '*') scanComment();
    }
    // end if c is not whitespace
    if(!isspace(*c)) break;
  }
  // scan a token  
  Position startPos = program.currentPosition();
  std::optional<TokenValue> tokenValue;

  if(auto c = program.currentChar()){
    if(isalpha(*c)){
      std::string id = scanIdentifier();
      // if id is a keyword, return the keyword
      if(auto keyword = isKeyword(id)){
        tokenValue = *keyword;
      }else{
        tokenValue = id;
      }
    }
    if(*c == '"') tokenValue = scanString();
  }

  Position endPos = program.currentPosition();
  program.move();

  if(auto value = tokenValue) return Token{startPos, endPos, *value};
  else {
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
  for(int i = 0; i<13; ++i) {
    if(ident == keywords[i]) return static_cast<Keyword>(i);
  }
}

std::string Scanner::scanIdentifier(){
  std::string ident = "";
  ident += program.currentChar().value();
  while(auto c = program.peekChar()){
    if(!isalnum(*c)) break;
    ident += *c;
    program.move();
  }
  return ident;
}


std::string Scanner::scanString(){
  std::string lexeme = "";
  std::string error = "";

  if( auto c = program.currentChar(); *c == '"'){
    program.peekChar();
  }

  while(program.currentChar() != '"'){
    if(auto c = program.currentChar()){

      if(c == '\n'){
        // ERROR
      }else if(c == '\\'){
        // escape characters
        if(auto c = program.peekChar()){
          lexeme += *c;
        }else{
          // ERRROR
        }
      }else{
        lexeme += *c;
      }
    }else{

    }


  }
  return lexeme;
}

