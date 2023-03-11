#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Util.h"
#include "Program.h"
#include "Visitor.h"
#include "Token.h"

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
class OpndAstNode : public AstNodeBase {
  public: 
    ExprValue value;
    Operand operand; 

    inline bool operator==(const OpndAstNode &other) const {
      return this->operand == other.operand;
    }


};


class DeclAstNode : public AstNodeBase {
  public: 
    std::string varId;
    Type type;
    std::optional<ExprAstNode> value;
};

class AssignAstNode : public AstNodeBase {
  public:
    std::string varId;
    ExprAstNode expr;
};



class ReadAstNode : public AstNodeBase {
  public: 
    std::string varId;
};

class PrintAstNode : public AstNodeBase {
  public:
    ExprAstNode expr;
};


class StatementsAstNode : public AstNodeBase {
  public:
    std::vector<AstNode> statements; 
};

class ForAstNode : public AstNodeBase {
  public:
    std::string varId;
    ExprAstNode startExpr, endExpr;
    StatementsAstNode statements;
};

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

template<class NodeType>
std::string astNodeName(){
  return nodenames[AstNode(std::in_place_type<NodeType>).index()];;
}
