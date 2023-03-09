#include <string>
#include <optional>
#include <variant>
#include "Util.h"
#include "Program.h"
#include "Error.h"

#pragma once

// These must be in the same order
enum class Keyword {
  Var = 0, For, End, In, Do, Read, Print, Int, String, Bool, 
  Assert, If, Else
};
constexpr int n_keywords = 13;
constexpr const char *keywords[] = {
  "var", "for" , "end" , "in" , "do" , "read" , "print" , 
  "int" , "string" , "bool" , "assert" , "if" , "else"
};

enum class Delimiter {
  Assign, Semicolon, Colon, Range, OpenParen, ClosedParen, Eof
};


typedef std::variant<
  Literal, VarIdent, Operator, Keyword, Delimiter
> TokenValue;

struct Token {
  Span span;
  TokenValue value;
};


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