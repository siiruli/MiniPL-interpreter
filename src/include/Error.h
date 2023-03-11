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
  inline virtual std::string description(){
    std::stringstream desc;
    desc << errorClass() << ": ";
    desc << shortDescription();
    desc << " at " << span;
    desc << " in " << context << " (" << contextSpan << ")";
    return desc.str(); 
  };
  Span span;
  Span contextSpan;
  std::string context;

  private:
    virtual std::string errorClass() = 0;
    virtual std::string shortDescription() = 0;
};
enum class ScanningErrorType {Eof, UnexpNewline, UnexpChar};
struct ScanningError : ErrorBase {
  ScanningErrorType type;
  std::optional<char> ch;
  Position pos;
  inline virtual std::string errorClass() { return "Lexical error"; }
  inline virtual std::string shortDescription(){
    std::stringstream desc;
    desc << "Unexpected ";
    if(type == ScanningErrorType::UnexpChar){
      desc << " character " << ch.value_or(' ');
    }else if(type == ScanningErrorType::Eof){
      desc << "end-of-file"; 
    }else if(type == ScanningErrorType::UnexpNewline){
      desc << "newline"; 
    }
    return desc.str();
  }
  
  
};
struct ParsingError : ErrorBase {
  Token token;
  std::string data;
  inline virtual std::string errorClass() { return "Syntax error"; }
  inline virtual std::string shortDescription(){ 
    std::stringstream desc;
    desc << "Unexpected " << token.value;
    return desc.str();
  }
};
enum class SemErrorType {Other, NotDeclared, ReDeclared, WrongScope, AssignConstant};
struct SemanticError : ErrorBase {
  VarIdent identifier;
  SemErrorType type;
  inline virtual std::string errorClass() { return "Semantic error"; }
  inline virtual std::string shortDescription(){ 
    switch (type)
    {
    case SemErrorType::NotDeclared:
      return "Variable " + identifier + " used without declaration"; 
    case SemErrorType::ReDeclared:
      return "Redeclaration of variable " + identifier;
    case SemErrorType::WrongScope:
      return "Declaration not allowed inside if or for";
    case SemErrorType::AssignConstant:
      return "Variable " + identifier + " is constant inside for-loop";
      break;
    default:
      return "Unkown";
    }
    return "";
  }

};
struct TypeError : ErrorBase {

  std::vector<Type> expected;
  std::vector<Type> got;

  inline virtual std::string errorClass() { return "Type error"; }
  inline virtual std::string shortDescription(){
    std::stringstream desc;
    desc << "Unexpected types. Expected: ";
    for(auto t : expected) desc << t << " ";
    desc << ", got: ";
    for(auto t : got) desc << t << " ";
    return desc.str();
  }
};

typedef std::variant<ScanningError, ParsingError, SemanticError, TypeError> Error;

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
    std::string lineStart(uint LineNumber);
    std::string lineString(uint lineNumber);
    std::string lineStart();

    inline int lineStartLength() {
      return std::to_string(program.size()).size() + 2;
    }

    std::string lineSkipped();
    void printSpan(Span span);
    
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
