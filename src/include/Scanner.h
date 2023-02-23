#include <string>
#include <optional>
#include <variant>
#include "Program.h"

enum class Operator {Add = 0, Sub, Mul, Div, Less, Equal, And, Not};
constexpr const char operators[] = {
  '+', '-', '*', '/', '<', '=', '&', '!'
};
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

enum class Punctuation {
  Assign, Semicolon, Colon, Range, OpenParen, ClosedParen, Eof
};

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
typedef std::variant<
  Literal, VarIdent, Operator, Keyword, Punctuation
> TokenValue;

struct Token {
  Position startPos;
  Position endPos;
  TokenValue value;
};



class Scanner {
  public:
    Scanner(std::string &programText);

    Token getToken();
    std::optional<Token> scanToken();
  
  private: 
    Token current;
    ProgramIterator program;
    std::string scanString();
    std::string scanIdentifier();
    int scanInteger();
    void scanComment();
    Operator scanOperator();
    Punctuation scanPunctuation();

    std::optional<Keyword> isKeyword(std::string &id);    

  

};