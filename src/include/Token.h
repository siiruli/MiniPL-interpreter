#pragma once
#include <ostream>
#include <optional>
#include <variant>
#include "Program.h"
#include "Util.h"

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

inline std::ostream & operator<<(std::ostream &os, Keyword kw){
  return os << "keyword '" << keywords[static_cast<int>(kw)] << "'";
}


enum class Delimiter {
  Assign, Semicolon, Colon, Range, OpenParen, ClosedParen, Eof
};
constexpr int n_delims = 7;
constexpr const char *delims[] = {
  ":=", ";", ":", "..", "(", ")", "EoF"
};
inline std::ostream & operator<<(std::ostream &os, Delimiter d){
  return os << "delimiter '" << delims[static_cast<int>(d)] << "'";
}


enum class Operator {
  Add = 0, Sub, Mul, Div, Less, Equal, And, Not, Identity
};
constexpr int n_operators = 9;
constexpr const char *operators[] = {
  "+", "-", "*", "/", "<", "=", "&", "!", ""
};
inline std::ostream & operator<<(std::ostream &os, Operator op){
  return os << "operator '" << operators[static_cast<int>(op)] << "'";
}

struct Literal {
  std::variant<int, std::string> value;
  inline bool operator==(const Literal &other) const {
    return value == other.value;
  }
  inline bool operator!=(const Literal &other) const {
    return !operator==(other);
  }
};

inline std::ostream & operator<<(std::ostream &os, Literal l){
  std::visit(overloaded {
    [&](int &arg){
      os << "integer literal '" << arg << "'";
    },
    [&](std::string &arg){
      os << "string literal \"";
      if(arg.size() < 30) os << arg;
      else os << arg.substr(0, 10) << "...";
      os << "\"";
    }
  }, l.value);
  return os;
}

typedef std::string VarIdent;


typedef std::variant<
  Literal, VarIdent, Operator, Keyword, Delimiter
> TokenValue;
inline std::ostream & operator<<(std::ostream &os, TokenValue val){
  return std::visit([&](auto &arg) -> std::ostream & {
    return os << arg;
  }, val);
}


struct Token {
  Span span;
  TokenValue value;
};

