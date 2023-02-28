#include "Scanner.h"
#include "AstNode.h"

class TokenIterator {
  public: 
    virtual Token currentToken() = 0;
    virtual Token nextToken() = 0;
};

class ScannerIterator : public TokenIterator {
  public: 
    ScannerIterator(Scanner &scanner);
    virtual Token currentToken() override;
    virtual Token nextToken() override;
  private:
    Scanner &scanner;
    Token token;

};

class Parser {
  public:
    Parser(TokenIterator &iterator);

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
    TokenIterator &it;

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