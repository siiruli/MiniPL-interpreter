#pragma once
#include <string>
#include <optional>
#include <variant>
#include "Token.h"
#include "Util.h"
#include "Program.h"
#include "Error.h"



/* Scans the program for lexical elements, producing tokens one by one. 
Gives lexical error to the error handler. */
class Scanner {
  public:
    Scanner(Program &program, ErrorHandler &handler);

    // Scans the next token from the program.
    Token getToken();
  
  private: 
    std::optional<Token> scanToken();
    Token current;
    ProgramIterator program;
    Position startOfToken;
    ErrorHandler &errorHandler;
    void raiseError(ScanningErrorType errortype, std::string context);    

    void scanComment();
    std::optional<VarIdent> scanIdentifier();
    std::optional<Literal> scanInteger();
    std::optional<Literal> scanString();
    std::optional<Operator> scanOperator();
    std::optional<Delimiter> scanPunctuation();

    std::optional<Keyword> isKeyword(std::string &id);    

    bool isOperatorChar(char c);
    bool isPunctChar(char c);

};