#include "Scanner.h"
#include "AstNode.h"

class Parser {
  public:
    Parser(Scanner &scanner);

    AstNode program();
    StatementsAstNode statements();
    AstNode statement();
    // ExprAstNode expression();
    // AstNode assignment();
    // AstNode declaration();
    // AstNode forStatement();
    // AstNode ifStatement();
    // AstNode readStatement();
    // AstNode printStatement();
    
  private:
    Scanner &scanner;
    AstNode astRoot;

    Token currentToken;
    void nextToken();

    AstNode makeAstNode();
    VarIdent matchIdent();
    Literal matchLiteral();
    
    void match(TokenValue expected);
};