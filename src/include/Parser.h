#pragma once
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

    StatementNode program();
    StatementsAstNode statements(bool isRoot = false);
    std::optional<StatementNode> statement();
    StatementNode assignment();
    StatementNode declaration();
    StatementNode forStatement();
    StatementNode ifStatement();
    StatementNode readStatement();
    StatementNode printStatement();
    ExprNode expression(int precedence = 0);
    ExprNode operand();
    
  private:
    StatementNode astRoot;
    TokenIterator &it;
    ErrorHandler &handler;


    template<class ExpectedType, class NodeType>
    ExpectedType match(NodeType &node);
    
    template<class NodeType>
    void match(const TokenValue expected, NodeType &node);

    std::vector<TokenValue> expectedVals;




    template<class NodeType>
    void addMeta(NodeType &node, Token token);

    template<class NodeType>
    void addMeta(NodeType &node, StatementNode &childNode);
    template<class NodeType>
    void addMeta(NodeType &node, ExprNode &childNode);
    template<class NT1, class NT2>
    void addMeta(NT1 &node, NT2 &childNode);


    template<class NodeType>
    void raiseError(NodeType &node, ParsingError error = ParsingError{});

    TokenClass classify(Token token);  
    
};