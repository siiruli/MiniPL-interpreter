#pragma once
#include <variant>
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include "Token.h"
#include "AstNode.h"

template<class ErrorType, class NodeType>
ErrorType createError(Span span, NodeType &node){
  ErrorType error;
  error.span = span;
  error.addContext(node);
  return error;
}

struct ErrorBase {

  template<class NodeType>
  inline void addContext(NodeType &node){
    contextSpan = node.span;
    context = NodeType::name;
  }
  inline virtual std::string description(){
    std::stringstream desc;
    desc << errorClass() << ": ";
    desc << shortDescription();
    desc << " at " << span;
    desc << "\n";
    desc << "Found in " << context << " (" << contextSpan << ")";
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
      desc << "character '" << ch.value_or(' ') << "'";
    }else if(type == ScanningErrorType::Eof){
      desc << "end-of-file"; 
    }else if(type == ScanningErrorType::UnexpNewline){
      desc << "newline"; 
    }
    return desc.str();
  }
  
  
};

inline std::string valueStr(TokenValue value){
  std::stringstream desc;
  if(std::holds_alternative<VarIdent>(value)) 
    desc << "identifier '" << value << "'";
  else desc << value;
  return desc.str();
}
struct ParsingError : ErrorBase {
  Token token;
  std::string data;

  std::optional<TokenValue> expected = std::nullopt;
  
  inline virtual std::string errorClass() { return "Syntax error"; }
  inline virtual std::string shortDescription(){ 
    std::stringstream desc;
    desc << "Unexpected " << valueStr(token.value);
    if(expected.has_value()) 
      desc << "(expected " << valueStr(expected.value()) << ")";
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
    desc << "Unexpected type. Expected: ";
    for(auto t : expected) desc << t << " ";
    desc << ", got: ";
    for(auto t : got) desc << t << " ";
    return desc.str();
  }
};

struct RuntimeError : ErrorBase {

  std::string data;
  inline virtual std::string errorClass() { return "Runtime error"; }
  inline virtual std::string shortDescription(){ 
    std::stringstream desc;
    desc << data;
    return desc.str();
  }
};

typedef std::variant<
  ScanningError, ParsingError, SemanticError, TypeError, RuntimeError
> Error;
