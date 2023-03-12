#include "Scanner.h"
#include "AstNode.h"

/** @file */ 
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
    std::optional<Token> token;

};

enum class TokenClass {StartOfStmt, EndOfStmt, EndKeyword, Eof, Unimportant};

class Parser {
  public:
    Parser(TokenIterator &iterator, ErrorHandler &handler);

    AstNode program();
    StatementsAstNode statements();
    std::optional<AstNode> statement();
    AstNode assignment();
    AstNode declaration();
    AstNode forStatement();
    AstNode ifStatement();
    AstNode readStatement();
    AstNode printStatement();
    ExprNode expression();
    ExprNode operand();
    
  private:
    AstNode astRoot;
    TokenIterator &it;
    ErrorHandler &handler;


    template<class ExpectedType, class NodeType>
    ExpectedType match(NodeType &node);
    
    template<class NodeType>
    void match(const TokenValue expected, NodeType &node);

    std::vector<TokenValue> expectedVals;

    template<class ExpectedType, class NodeType>
    std::optional<ExpectedType> expect(NodeType &node);
    
    template<class NodeType>
    bool expect(const TokenValue expected, NodeType &node);


    template<class NodeType>
    void addMeta(NodeType &node, Token token);

    template<class NodeType>
    void addMeta(NodeType &node, AstNode &childNode);
    template<class NodeType>
    void addMeta(NodeType &node, ExprNode &childNode);
    template<class NT1, class NT2>
    void addMeta(NT1 &node, NT2 &childNode);


    template<class NodeType>
    void raiseError(NodeType &node);

    TokenClass classify(Token token);  
    
};