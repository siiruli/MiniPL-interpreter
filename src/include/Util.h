#pragma once
#include <string>
#include <variant>

/** @file */ 

// integer type
using num = long long;
inline num stringToNum(std::string s){
  return stoll(s); 
}

typedef std::variant<num, bool, std::string> ExprValue;

template<class... Ts> struct overloaded : Ts... { 
  using Ts::operator()...; 
};
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
 
