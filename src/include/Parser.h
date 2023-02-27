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
    AstNode assignment();
    // AstNode declaration();
    // AstNode forStatement();
    // AstNode ifStatement();
    // AstNode readStatement();
    // AstNode printStatement();
    // ExprAstNode expression();
    
  private:
    AstNode astRoot;
    TokenIterator it;

    template<class NodeType>
    VarIdent matchIdent(NodeType &node);

    template<class NodeType>
    Literal matchLiteral(NodeType &node);
    
    template<class NodeType>
    void match(TokenValue expected, NodeType &node);

    template<class NodeType>
    void addMeta(NodeType &node, Token token);

    template<class NodeType>
    void addMeta(NodeType &node, AstNode &childNode);
};