#include <variant>
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include "Program.h"

#pragma once


struct ErrorBase {
  virtual std::string description() = 0;
  Span span;
  std::string context;
};
enum class ScanningErrorType {Eof, UnexpChar};
struct ScanningError : ErrorBase {
  ScanningErrorType type;
  std::optional<char> ch;
  Position pos;

  inline virtual std::string description() {
    std::stringstream desc;
    desc << "Lexical error: ";
    if(type == ScanningErrorType::UnexpChar){
      desc << "Unexpected character " << ch.value_or(' ');
      desc << " at " << pos;
      desc << " while scanning " << context;
    }else if(type == ScanningErrorType::Eof){
      desc << "Unexpected end-of-file at " << pos; 
      desc << " while scanning " << context;
    }
    return desc.str();
  }
  
  
};
struct ParsingError : ErrorBase {
  std::string data;

  inline virtual std::string description() {
    std::stringstream desc;
    desc << "Syntax error: ";
    return desc.str();
  }
};
typedef std::variant<ScanningError, ParsingError> Error;

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
