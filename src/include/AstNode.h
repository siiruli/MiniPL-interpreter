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

class AstNodeBase {
  public:
    Span span;
};

class ExprAstNode;
class DeclAstNode; 
class AssignAstNode; 
class ForAstNode;
class IfAstNode;
class ReadAstNode;
class PrintAstNode;
class StatementsAstNode;
class OpndAstNode;

typedef std::variant<
  DeclAstNode, 
  AssignAstNode, 
  ForAstNode, 
  IfAstNode, 
  ReadAstNode, 
  PrintAstNode,
  StatementsAstNode,
  ExprAstNode,
  OpndAstNode> AstNode;
constexpr const char *nodenames[] = {
  "declaration", "assignment", "for", "if", "read", "print", 
  "statement", "expression", "operand"
};


/*! \ingroup AST */
class ExprAstNode : public AstNodeBase {
  public: 
    ExprValue value;
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
class OpndAstNode : public AstNodeBase {
  public: 
    ExprValue value;
    Operand operand; 

    inline bool operator==(const OpndAstNode &other) const {
      return this->operand == other.operand;
    }


};


/*! \ingroup AST */
class DeclAstNode : public AstNodeBase {
  public: 
    std::string varId;
    Type type;
    std::optional<ExprAstNode> value;
};

/*! \ingroup AST */
class AssignAstNode : public AstNodeBase {
  public:
    std::string varId;
    ExprAstNode expr;
};


/*! \ingroup AST */
class ReadAstNode : public AstNodeBase {
  public: 
    std::string varId;
};

/*! \ingroup AST */
class PrintAstNode : public AstNodeBase {
  public:
    ExprAstNode expr;
};


/*! \ingroup AST */
class StatementsAstNode : public AstNodeBase {
  public:
    std::vector<AstNode> statements; 
};

/*! \ingroup AST */
class ForAstNode : public AstNodeBase {
  public:
    std::string varId;
    ExprAstNode startExpr, endExpr;
    StatementsAstNode statements;
};

/*! \ingroup AST */
class IfAstNode : public AstNodeBase {
  public:
    ExprAstNode expr;
    StatementsAstNode ifStatements, elseStatements;
};

inline AstNodeBase & getBaseReference(AstNode &node){
  AstNodeBase &base = std::visit([](auto &node) -> AstNodeBase& {
    return static_cast<AstNodeBase&>(node);
  }, node);
  return base;
}

/*! \ingroup AST */
template<class NodeType>
std::string astNodeName(){
  return nodenames[AstNode(std::in_place_type<NodeType>).index()];;
}
