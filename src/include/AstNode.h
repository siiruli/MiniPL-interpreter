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

class ExprAstNode : AstNodeBase {
  public: 
    ExprValue value;
    Operator op;

};



class DeclAstNode : AstNodeBase {
  public: 
    std::string varId;
    Type type;
};

class AssignAstNode : AstNodeBase {
  public:
    std::string varId;
    ExprAstNode expr;
};



class ReadAstNode : AstNodeBase {
  public: 
    std::string varId;
};

class PrintAstNode : AstNodeBase {
  public:
    ExprAstNode expr;
};

class ErrorAstNode : AstNodeBase {

};

class StatementsAstNode;
class ForAstNode;
class IfAstNode;

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

class StatementsAstNode : AstNodeBase {
  public:
    std::vector<AstNode> statements; 
};

class ForAstNode : AstNodeBase {
  public:
    std::string varId;
    ExprAstNode startExpr, endExpr;
    StatementsAstNode statements;
};

class IfAstNode : AstNodeBase {
  public:
    ExprAstNode expr;
    StatementsAstNode ifStatements, elseStatements;
};

