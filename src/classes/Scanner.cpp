#include "Scanner.h"


#include <iostream>
#include <vector>

Scanner::Scanner(std::string &program) : program(program){}

std::optional<char> Scanner::currentChar(){
  if(pos.charIndex < program.size()) 
    return program[pos.charIndex];
  else return {};
}

std::optional<char> Scanner::nextChar(){
  if(currentChar() == '\n') ++pos.lineNumber;
  ++pos.charIndex;
  return currentChar();
}

Token getToken(){
  
}

std::string Scanner::scanString(){
  std::string lexeme = "";
  std::string error = "";

  if( auto c = currentChar(); *c == '"'){
    nextChar();
  }
  else{
    // ERROR
  }

  while(currentChar() != '"'){
    if(auto c = currentChar()){

      if(c == '\n'){
        // ERROR
      }else if(c == '\\'){
        // escape characters
        if(auto c = nextChar()){
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

