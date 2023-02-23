#include <string>
#include <vector>
#include "Visitor.h"
#include "Value.h"
// #include "Scanner.h"

enum Type {Int, Bool, String};
enum class Operator2 {Add = 0, Sub, Mul, Div, Less, Equal, And, Not};


class ExprAstNode {
  public: 
    Value value;
    Operator2 op;
};



class DeclAstNode {
  public: 
    std::string varId;
    Type type;
};

class AssignAstNode {
  public:
    std::string varId;
    ExprAstNode expr;
};



class ReadAstNode {
  public: 
    std::string varId;
};

class PrintAstNode {
  public:
    ExprAstNode expr;
};

class ErrorAstNode {

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

class StatementsAstNode {
  public:
    std::vector<AstNode> statements; 
};

class ForAstNode {
  public:
    std::string varId;
    ExprAstNode startExpr, endExpr;
    StatementsAstNode statements;
};

class IfAstNode {
  public:
    ExprAstNode expr;
    StatementsAstNode ifStatements, elseStatements;
};

