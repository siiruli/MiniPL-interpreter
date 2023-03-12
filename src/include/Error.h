#pragma once
#include <variant>
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include "Program.h"
#include "Token.h"
#include "ErrorTypes.h"

/** @file */ 


class ErrorMessager {
  public:
    inline ErrorMessager(Program &program, std::ostream &output) 
      : program(program), output(output) {}

    void printError(Error &error);
    void printError(ErrorBase &error);

  private:
    Program &program;
    std::ostream &output;
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

    void raiseError(Error error);
    bool hasErrors();

    uint errorNumber();
    void printErrors(Program &program, std::ostream &ostream);

  private:
    std::vector<Error> errors;


};
