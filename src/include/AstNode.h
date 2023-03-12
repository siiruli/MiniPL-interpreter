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

enum Type {Int, Bool, String, Void, Broken};
constexpr const char *typenames[] = {
  "int", "bool", "string", "void", "broken"
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
  StatementsAstNode> AstNode;

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
  Literal literal;
};
struct VarNode : AstNodeBase { 
  VarIdent varId;
};
struct UnaryOp : AstNodeBase {
  Operator op;
  std::unique_ptr<ExprNode> opnd;
};
struct BinaryOp : AstNodeBase {
  Operator op;
  std::unique_ptr<ExprNode> opnd1, opnd2;
};

/*! \ingroup AST */
struct ExprAstNode : AstNodeBase { 

  std::unique_ptr<OpndAstNode> opnd1;
  Operator op;
  inline bool operator==(const ExprAstNode &other) const {
    return this->op == other.op &&
      opnd1 == other.opnd1;
  }
  // std::optional<std::unique_ptr<OpndAstNode>> opnd2;
  std::unique_ptr<OpndAstNode> opnd2;

};
typedef std::variant<
  Literal, 
  VarIdent, 
  ExprAstNode> Operand;

/*! \ingroup AST */
struct OpndAstNode : AstNodeBase { 

  Operand operand; 

  inline bool operator==(const OpndAstNode &other) const {
    return this->operand == other.operand;
  }


};


/*! \ingroup AST */
struct DeclAstNode : AstNodeBase { 
  std::string varId;
  Type type;
  std::optional<ExprNode> value;
};

/*! \ingroup AST */
struct AssignAstNode : AstNodeBase {
  std::string varId;
  ExprNode expr;
};


/*! \ingroup AST */
struct ReadAstNode : AstNodeBase { 
  std::string varId;
};

/*! \ingroup AST */
struct PrintAstNode : AstNodeBase {
  ExprNode expr;
};


/*! \ingroup AST */
struct StatementsAstNode : AstNodeBase {
  std::vector<AstNode> statements; 
};

/*! \ingroup AST */
struct ForAstNode : AstNodeBase {
  VarNode var;
  ExprNode startExpr, endExpr;
  StatementsAstNode statements;
};

/*! \ingroup AST */
struct IfAstNode : AstNodeBase {
  ExprNode expr;
  StatementsAstNode ifStatements, elseStatements;
};

inline AstNodeBase & getBaseReference(AstNode &node){
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
/*! \ingroup AST */
template<class NodeType>
std::string astNodeName(){
  return ""; //nodenames[AstNode(std::in_place_type<NodeType>).index()];;
}
