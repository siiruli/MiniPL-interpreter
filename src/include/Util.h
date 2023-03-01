#pragma once
#include <string>
#include <variant>

enum class Operator {
  Add = 0, Sub, Mul, Div, Less, Equal, And, Not, Identity
};

typedef std::variant<int, bool, std::string> ExprValue;

template<class... Ts> struct overloaded : Ts... { 
  using Ts::operator()...; 
};
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
 

struct Literal {
  std::variant<int, std::string> value;
  inline bool operator==(const Literal &other) const {
    return value == other.value;
  }
  inline bool operator!=(const Literal &other) const {
    return !operator==(other);
  }
};

typedef std::string VarIdent;


// struct Value {
//   Type type;
//   union {
//     int Int;
//     bool Bool;
//     std::string String;
//   } value;
// };