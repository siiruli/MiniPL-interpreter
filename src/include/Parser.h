#include "Scanner.h"
#include "AstNode.h"

class TokenIterator {
  public: 
    TokenIterator(Scanner &scanner);
    Token currentToken();
    Token nextToken();
  private:
    Scanner &scanner;
    Token token;
};

class Parser {
  public:
    Parser(Scanner &scanner);

    AstNode program();
    StatementsAstNode statements();
    std::optional<AstNode> statement();
    // ExprAstNode expression();
    // AstNode assignment();
    // AstNode declaration();
    // AstNode forStatement();
    // AstNode ifStatement();
    // AstNode readStatement();
    // AstNode printStatement();
    
  private:
    AstNode astRoot;
    TokenIterator it;

    AstNode makeAstNode();
    VarIdent matchIdent();
    Literal matchLiteral();
    
    void match(TokenValue expected);
};