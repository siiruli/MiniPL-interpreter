#include <variant>
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include "Program.h"
#include "Token.h"
#include "AstNode.h"
#pragma once


struct ErrorBase {
  virtual std::string description() = 0;
  Span span;
  std::string context;
};
enum class ScanningErrorType {Eof, UnexpNewline, UnexpChar};
struct ScanningError : ErrorBase {
  ScanningErrorType type;
  std::optional<char> ch;
  Position pos;

  inline virtual std::string description() {
    std::stringstream desc;
    desc << "Lexical error: ";
    desc << "Unexpected ";
    if(type == ScanningErrorType::UnexpChar){
      desc << " character " << ch.value_or(' ');
    }else if(type == ScanningErrorType::Eof){
      desc << "end-of-file"; 
    }else if(type == ScanningErrorType::UnexpNewline){
      desc << "newline"; 
    }
    desc << " at " << pos;
    desc << " while scanning " << context;

    return desc.str();
  }
  
  
};
struct ParsingError : ErrorBase {
  Token token;
  std::string data;

  inline virtual std::string description() {
    std::stringstream desc;
    desc << "Syntax error: Unexpected " << token.value;
    desc << " at " << token.span;
    return desc.str();
  }
};
struct SemanticError : ErrorBase {
  
  inline virtual std::string description() {
    std::stringstream desc;
    desc << "Semantic error in " << context;
    desc << " (" << span << ")";
    return desc.str();
  }

};
struct TypeError : SemanticError {
  std::vector<Type> expected;
  std::vector<Type> got;
  inline virtual std::string description() {
      std::stringstream desc;
      desc << SemanticError::description();
      desc << ". expected: ";
      for(auto t : expected) desc << t << " ";
      desc << ", got: ";
      for(auto t : got) desc << t << " ";
      return desc.str();
    }
};

typedef std::variant<ScanningError, ParsingError, TypeError> Error;

// typedef std::variant<ScanningError, ParsingError> ErrorType;

// inline std::ostream & operator<<(std::ostream &os, ScanningError error){
//   return os << "Lexical error: " << int(error);
// }
// inline std::ostream & operator<<(std::ostream &os, ParsingError error){
//   return os << "Syntax error: " << error.data;
// }
// inline std::ostream & operator<<(std::ostream &os, ErrorType error){
//   return std::visit([&](auto &arg) -> std::ostream & {
//     return os << arg; 
//   }, error);
// }




class ErrorMessager {
  public:
    inline ErrorMessager(Program &program) : program(program) {}

    void printError(Error &error);
    void printError(ErrorBase &error);

    std::ostream &output = std::cout;
  private:
    Program &program;
};

class ErrorHandler {
  public:
    inline ErrorHandler(Program &program) : messager(program) {}

    void raiseError(Error error);
    bool hasErrors();
  private:
    std::vector<Error> errors;
    ErrorMessager messager;
};
