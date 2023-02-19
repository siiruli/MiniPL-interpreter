#include <string>
#include "Visitor.h"
class AstNode {
  public: 
    const void visit(Visitor * visitor);

};

class ExprAstNode : AstNode{
  public: 
    int value;
    const void visit(Visitor * visitor);
};


class StatementsAstNode : AstNode{
  public:
    const void visit(Visitor * visitor);
};

class DeclAstNode : AstNode{
  public: 
    std::string varId;
    int type;
};

class AssignAstNode : AstNode{
  public:
    std::string varId;
    ExprAstNode expr;
};

class ForAstNode : AstNode{
  public:
    std::string varId;
    ExprAstNode startExpr, endExpr;
    StatementsAstNode statements;
};

class IfAstNode : AstNode{
  public:
    ExprAstNode expr;
    StatementsAstNode ifStatements, elseStatements;
};

class ReadAstNode : AstNode{
  public: 
    std::string varId;
};

class PrintAstNode : AstNode{
  public:
    ExprAstNode expr;
};

