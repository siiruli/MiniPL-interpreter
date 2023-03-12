#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Util.h"
#include "Program.h"
#include "Visitor.h"
#include "Token.h"

/** @file */ 
/** \defgroup AST AST nodes*/

enum Type {Int, Bool, String, Broken};
constexpr const char *typenames[] = {
  "int", "bool", "string", "broken"
};
inline std::ostream & operator<<(std::ostream &os, Type arg){
  return os << "'" << typenames[static_cast<int>(arg)] << "'";
}

struct AstNodeBase {
    Span span;
};

struct ExprAstNode;
struct DeclAstNode; 
struct AssignAstNode; 
struct ForAstNode;
struct IfAstNode;
struct ReadAstNode;
struct PrintAstNode;
struct StatementsAstNode;
struct OpndAstNode;

typedef std::variant<
  DeclAstNode, 
  AssignAstNode, 
  ForAstNode, 
  IfAstNode, 
  ReadAstNode, 
  PrintAstNode,
  StatementsAstNode> StatementNode;

struct LiteralNode;
struct VarNode;
struct UnaryOp;
struct BinaryOp;
typedef std::variant<
  LiteralNode, 
  VarNode,
  UnaryOp,
  BinaryOp> ExprNode;

struct LiteralNode : AstNodeBase { 
  static constexpr auto name = "literal";
  Literal literal;
};
struct VarNode : AstNodeBase { 
  static constexpr auto name = "variable";
  VarIdent varId;
};
struct UnaryOp : AstNodeBase {
  static constexpr auto name = "expression";
  Operator op;
  std::unique_ptr<ExprNode> opnd;
};
struct BinaryOp : AstNodeBase {
  static constexpr auto name = "expression";
  Operator op;
  std::unique_ptr<ExprNode> opnd1, opnd2;
};


/*! \ingroup AST */
struct DeclAstNode : AstNodeBase { 
  static constexpr auto name = "declaration";
  VarIdent varId;
  Type type;
  std::optional<ExprNode> value;
};

/*! \ingroup AST */
struct AssignAstNode : AstNodeBase {
  static constexpr auto name = "assignment";
  std::string varId;
  ExprNode expr;
};


/*! \ingroup AST */
struct ReadAstNode : AstNodeBase { 
  static constexpr auto name = "read-statement";
  std::string varId;
};

/*! \ingroup AST */
struct PrintAstNode : AstNodeBase {
  static constexpr auto name = "print-statement";
  ExprNode expr;
};


/*! \ingroup AST */
struct StatementsAstNode : AstNodeBase {
  static constexpr auto name = "statements";
  std::vector<StatementNode> statements; 
};

/*! \ingroup AST */
struct ForAstNode : AstNodeBase {
  static constexpr auto name = "for-statement";
  VarNode var;
  ExprNode startExpr, endExpr;
  StatementsAstNode statements;
};

/*! \ingroup AST */
struct IfAstNode : AstNodeBase {
  static constexpr auto name = "if-statement";
  ExprNode expr;
  StatementsAstNode ifStatements, elseStatements;
};

inline AstNodeBase & getBaseReference(StatementNode &node){
  AstNodeBase &base = std::visit([](auto &node) -> AstNodeBase& {
    return static_cast<AstNodeBase&>(node);
  }, node);
  return base;
}
inline AstNodeBase & getBaseReference(ExprNode &node){
  AstNodeBase &base = std::visit([](auto &node) -> AstNodeBase& {
    return static_cast<AstNodeBase&>(node);
  }, node);
  return base;
}
