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
constexpr const char *keywords[] = {
  "var", "for" , "end" , "in" , "do" , "read" , "print" , 
  "int" , "string" , "bool" , "assert" , "if" , "else"
};

enum class Punctuation {
  Assign, Semicolon, Colon, Range, OpenParen, ClosedParen
};

enum class CharType {Letter, Digit, Punct, Operator};

typedef std::variant<
  int, std::string, Operator, Keyword, Punctuation
> TokenValue;

struct Token {
  Position startPos;
  Position endPos;
  TokenValue value;
};



class Scanner {
  public:
    Scanner(std::string &programText);

    std::optional<Token> getToken();
  private: 
    ProgramIterator program;
    std::string scanString();
    std::string scanIdentifier();
    int scanInteger();
    void scanComment();
    Operator scanOperator();
    Punctuation scanPunctuation();

    std::optional<Keyword> isKeyword(std::string &id);    


    CharType characterType(char c);

  

};