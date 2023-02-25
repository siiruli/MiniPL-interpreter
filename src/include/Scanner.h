#include <string>
#include <optional>
#include <variant>
#include "Util.h"
#include "Program.h"
#include "Error.h"
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
    Scanner(std::string &programText, ErrorHandler &handler);

    Token getToken();
    std::optional<Token> scanToken();
  
  private: 
    Token current;
    ProgramIterator program;
    ErrorHandler &errorHandler;
    
    void scanComment();
    std::optional<VarIdent> scanIdentifier();
    std::optional<Literal> scanInteger();
    std::optional<Literal> scanString();
    std::optional<Operator> scanOperator();
    std::optional<Punctuation> scanPunctuation();

    std::optional<Keyword> isKeyword(std::string &id);    

    bool isOperatorChar(char c);
    bool isPunctChar(char c);
    bool isAlpha(char c);
    bool isDigit(char c);
    bool isSpace(char c);    
  

};