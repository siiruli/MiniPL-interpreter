#include <string>
#include <optional>
#include <variant>
#include "Span.h"

enum class Operator {Add, Sub, Mul, Div, Less, Equal, And, Not};
// These must be in the same order
enum class Keyword {
  Var = 0, For, End, In, Do, Read, Print, Int, String, Bool, 
  Assert, If, Else
};
constexpr const char *keywords[] = {
  "var", "for" , "end" , "in" , "do" , "read" , "print" , 
  "int" , "string" , "bool" , "assert" , "if" , "else"
};

enum class Punctuation {Assign, Semicolon, Colon, Range};

struct Token {
  Position position;
  std::variant<
    int, std::string, Operator, Keyword, Punctuation
  > value;
};



class Scanner {
  public:
    Scanner(std::string &program);

    Token getToken();
  private: 
    std::string &program;
    Position pos;

    std::optional<char> nextChar();
    std::optional<char> currentChar();
    
    
    std::string scanString();
    void scanComment();




  

};