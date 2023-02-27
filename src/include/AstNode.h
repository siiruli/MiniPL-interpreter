#include <string>
#include <vector>
#include "Util.h"
#include "Program.h"
#include "Visitor.h"

enum Type {Int, Bool, String};

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
class ErrorAstNode;

typedef std::variant<
  ExprAstNode,
  DeclAstNode, 
  AssignAstNode, 
  ForAstNode, 
  IfAstNode, 
  ReadAstNode, 
  PrintAstNode,
  StatementsAstNode,
  ErrorAstNode> AstNode;





class ExprAstNode : public AstNodeBase {
  public: 
    ExprValue value;
    Operator op;

};



class DeclAstNode : public AstNodeBase {
  public: 
    std::string varId;
    Type type;
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

class ErrorAstNode : public AstNodeBase {

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

inline AstNodeBase & getBaseReference(AstNode node){
  AstNodeBase &base = std::visit([](auto &node) -> AstNodeBase& {
    return static_cast<AstNodeBase&>(node);
  }, node);
  return base;
}