#include <variant>
#include <vector>
#include <iostream>
#include <ostream>
#include "Program.h"

#pragma once

enum class ScanningError {UnexpChar, Eof};

struct ParsingError {
  std::string data;
};

typedef std::variant<ScanningError, ParsingError> ErrorType;

inline std::ostream & operator<<(std::ostream &os, ScanningError error){
  return os << "Lexical error: " << int(error);
}
inline std::ostream & operator<<(std::ostream &os, ParsingError error){
  return os << "Syntax error: " << error.data;
}
inline std::ostream & operator<<(std::ostream &os, ErrorType error){
  return std::visit([&](auto &arg) -> std::ostream & {
    return os << arg; 
  }, error);
}

struct Error {
  Span programPosition;
  ErrorType type;
  std::string info;
};


class ErrorMessager {
  public:
    void printError(Error error);

    std::ostream &output = std::cout;
};

class ErrorHandler {
  public:
    void raiseError(Error error);
    bool hasErrors();
  private:
    std::vector<Error> errors;
    ErrorMessager messager;
};
