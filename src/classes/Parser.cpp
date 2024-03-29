#include <variant>
#include <type_traits>
#include <exception>
#include "Parser.h"

ScannerIterator::ScannerIterator(Scanner &scanner) 
  : scanner(scanner)
{
}

Token ScannerIterator::nextToken(){
  token = scanner.getToken();
  return token.value();
}
Token ScannerIterator::currentToken(){
  if(!token.has_value()) token = scanner.getToken();
  return token.value();
}

struct ParserException : public std::exception {
   const char * what () const throw () {
      return "Unexpected token";
   }
};

Parser::Parser(TokenIterator &it, ErrorHandler &handler) 
  : it(it), handler(handler) {};


StatementNode Parser::program(){
  StatementsAstNode stmts;
  // try {
    stmts = statements(true);
  try{
    match(Delimiter::Eof, stmts);
  }catch(ParserException &e){
    std::cout << "Expected EoF, got " << it.currentToken().value << "\n"; 
  }    
  return stmts;
}

StatementsAstNode Parser::statements(bool isRoot){
  StatementsAstNode node;
  while(true){
    try {
      if(auto stmt = statement()){
        node.statements.push_back(std::move(*stmt));
        addMeta(node, stmt.value());
      }else{
        if(isRoot && classify(it.currentToken()) == TokenClass::EndKeyword){
          // skip end for and end if
          it.nextToken();
          Token token = it.currentToken();
          if(token.value == TokenValue{Keyword::For} 
            || token.value == TokenValue{Keyword::If}){
            it.nextToken();
          }
          if(it.currentToken().value == TokenValue{Delimiter::Semicolon}){
            it.nextToken();
          }
          continue;
        }
        else break;
      }
    } catch(ParserException &e){
      while(classify(it.currentToken()) == TokenClass::Unimportant){
        it.nextToken();
      }
      auto c = classify(it.currentToken());
      if(c == TokenClass::EndKeyword){
        if(!isRoot) return node;
        else continue;
      }
      if(c == TokenClass::EndOfStmt) it.nextToken();
    }
  }
  return node;
}

TokenClass Parser::classify(Token token){
  TokenClass c;
  std::visit(overloaded {
    [&](VarIdent &arg){
      c = TokenClass::Unimportant;
    },
    [&](Keyword &arg){
      switch (arg)
      {
        case Keyword::Var:
        case Keyword::For:
        case Keyword::If:
        case Keyword::Print:
        case Keyword::Read:
          c = TokenClass::StartOfStmt;
          break;
        case Keyword::End:
          c = TokenClass::EndKeyword;
          break; 
        default:
          c = TokenClass::Unimportant;
          break;
      }
    },
    [&](Delimiter &arg){
      switch (arg)
      {
        case Delimiter::Eof: c = TokenClass::Eof; break;
        case Delimiter::Semicolon: c = TokenClass::EndOfStmt; break;
        default: c = TokenClass::Unimportant;
      }
    },
    [&](auto &arg){
      // epsilon rule
      c = TokenClass::Unimportant;
    }
  }, token.value);
  return c;
}

std::optional<StatementNode> Parser::statement(){
  std::optional<StatementNode> node; 
  Token token = it.currentToken();
  std::visit( overloaded {
    [&](VarIdent &arg){
      node = assignment();
    },
    [&](Keyword &arg){
      switch (arg)
      {
        case Keyword::Var:
          node = declaration();
          break;
        case Keyword::For:
          node = forStatement();
          break;
        case Keyword::If:
          node = ifStatement();
          break;
        case Keyword::Print:
          node = printStatement();
          break;
        case Keyword::Read:
          node = readStatement();
          break;
        default:
          // epsilon rule
          node = std::nullopt;
      }
    },
    [&](auto &arg){
      // epsilon rule
      node = std::nullopt;
    }
  }, token.value);
  
  if(node.has_value()){
    std::visit([&](auto &node){
      match(Delimiter::Semicolon, node);
    }, node.value());
  }

  return node;
}

StatementNode Parser::assignment(){
  AssignAstNode node;
  node.varId = match<VarIdent>(node);

  match(Delimiter::Assign, node);
  node.expr = expression();
  addMeta(node, node.expr);

  return node;
}

StatementNode Parser::declaration(){
  DeclAstNode node;
  match(Keyword::Var, node);
  node.varId = match<VarIdent>(node);
  match(Delimiter::Colon, node);
  Keyword kw = match<Keyword>(node);
  switch (kw)
  {
  case Keyword::Int: node.type = Type::Int; break;
  case Keyword::Bool: node.type = Type::Bool; break;
  case Keyword::String: node.type = Type::String; break;  
  default:
    // Error
    break;
  }
  if(it.currentToken().value == TokenValue{Delimiter::Assign}){
    match(Delimiter::Assign, node);
    node.value = expression();
  }
  return node;
}

StatementNode Parser::forStatement(){
  ForAstNode node;
  match(Keyword::For, node);
  node.var.varId = match<VarIdent>(node.var);
  match(Keyword::In, node);
  node.startExpr = expression();
  match(Delimiter::Range, node);
  node.endExpr = expression();
  match(Keyword::Do, node);
  node.statements = statements();
  match(Keyword::End, node);
  match(Keyword::For, node);
  return node;
}
StatementNode Parser::ifStatement(){
  IfAstNode node;
  match(Keyword::If, node);
  node.expr = expression();
  match(Keyword::Do, node);
  node.ifStatements = statements();
  addMeta(node, node.ifStatements);

  if(it.currentToken().value == TokenValue{Keyword::Else}){
    match(Keyword::Else, node);
    node.elseStatements = statements();
    addMeta(node, node.elseStatements);

  }
  match(Keyword::End, node);
  match(Keyword::If, node);

  return node;
}
StatementNode Parser::readStatement(){
  ReadAstNode node;
  match(Keyword::Read, node);
  node.varId = match<VarIdent>(node);
  return node;
}
StatementNode Parser::printStatement(){
  PrintAstNode node;
  match(Keyword::Print, node);
  node.expr = expression();
  // addMeta(node, node.expr);
  return node;
}
bool isUnaryOp(TokenValue op){
  return op == TokenValue{Operator::Not} || 
    op == TokenValue{Operator::Identity};
}
const int op_max_precedence = 5;
int getPrecedence(Operator op){
    switch (op)
  {
    case Operator::Identity:
    case Operator::Not:
      return 5;
    case Operator::Mul: 
    case Operator::Div: 
      return 4;
    case Operator::Sub: 
    case Operator::Add: 
      return 3;
    case Operator::Less:
      return 2;
    case Operator::Equal: 
      return 1;
    case Operator::And:
      return 0;
    break;
  }
  return 0;
}
int getPrecedence(TokenValue op){
  if(std::holds_alternative<Operator>(op)){
    return getPrecedence(std::get<Operator>(op));
  }else return -1;
}



ExprNode Parser::expression(int precedence){
  if(precedence > op_max_precedence) return operand();
  Token token = it.currentToken();
  if(isUnaryOp(token.value) 
    && getPrecedence(token.value) == precedence){
    UnaryOp node;
    node.op = match<Operator>(node);
    node.opnd = std::make_unique<ExprNode>(expression(precedence));
    addMeta(node, *node.opnd);
    return node;
  }else{
    
    ExprNode opnd = expression(precedence+1);

    while(std::holds_alternative<Operator>(it.currentToken().value) 
        && getPrecedence(it.currentToken().value) == precedence){
      BinaryOp node;
      if(isUnaryOp(it.currentToken().value)){
        raiseError(node);
      }
      node.opnd1 = std::make_unique<ExprNode>(std::move(opnd));
      addMeta(node, *node.opnd1);
      node.op = match<Operator>(node);
      node.opnd2 = std::make_unique<ExprNode>(expression(precedence+1));
      addMeta(node, *node.opnd2);
      opnd = std::move(node);
    }
    return opnd;
  }
}
ExprNode Parser::operand(){
  Token token = it.currentToken();
  ExprNode node = std::visit( overloaded  {
    [&](VarIdent &arg) -> ExprNode {
      // VarIdent
      VarNode node;
      node.varId = match<VarIdent>(node);
      return node;
    },
    [&](Literal &arg) -> ExprNode {
      // Literal
      LiteralNode node;
      node.literal = match<Literal>(node);
      return node;
    },
    [&](Delimiter &arg) -> ExprNode {
      // (
      UnaryOp node;
      match(Delimiter::OpenParen, node);
      node.op = Operator::Identity;
      node.opnd = std::make_unique<ExprNode>(expression());
      addMeta(node, *node.opnd);
      match(Delimiter::ClosedParen, node);
      return node;
    },
    [&](auto &arg) -> ExprNode {
      // epsilon rule
      UnaryOp node;
      raiseError(node);
      throw ParserException();
    }
  }, token.value);
  return node;
}
// Add metadata from a token to an AST node
template<class NodeType>
void Parser::addMeta(NodeType &node, Token token){
  node.span += token.span;
  // AstNodeBase &base = getBaseReference(node);
  // base.span += token.span;
}
// Add metadata from a child AST node to its parent
template<class NodeType>
void Parser::addMeta(NodeType &node, StatementNode &childNode){
  AstNodeBase &childBase = getBaseReference(childNode);
  node.span += childBase.span;
}
template<class NodeType>
void Parser::addMeta(NodeType &node, ExprNode &childNode){
  AstNodeBase &childBase = getBaseReference(childNode);
  node.span += childBase.span;
}

// Add metadata from a child AST node to its parent
template<class NT1, class NT2>
void Parser::addMeta(NT1 &node, NT2 &childNode){
  node.span += childNode.span;
}
// Check that current token has the expected value, 
// and move to next token.
// Also add metadata from token to node.
template<class NodeType>
void Parser::match(TokenValue expected, NodeType &node){
  Token token = it.currentToken();
  if(token.value == expected){
    addMeta(node, it.currentToken());
    it.nextToken();
  }else{
    // ERROR
    ParsingError error;
    error.expected = expected;
    raiseError(node, error);
    throw ParserException();
    // exit(1);
  }
}

// Check thet current token has the expected type, 
// return the value, and move to next token.
// Also add metadata from token to node.
template<class ExpectedType, class NodeType>
ExpectedType Parser::match(NodeType &node){
  Token token = it.currentToken();
  if(std::holds_alternative<ExpectedType>(token.value)){
    addMeta(node, token);
    it.nextToken();
    return std::get<ExpectedType>(token.value);
  }else{
    // Error
    // return it.currentToken();
    
    raiseError(node);
    throw ParserException();

    // exit(1);
  }
}

template<class NodeType>
void Parser::raiseError(NodeType &node, ParsingError error){
  error.token = it.currentToken();
  error.span = error.token.span;
  error.addContext(node);
  error.contextSpan += error.span;
  handler.raiseError(error);
}
