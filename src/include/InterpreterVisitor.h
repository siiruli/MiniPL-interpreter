#include <map>
#include "Util.h"
#include "Visitor.h"
#include "AstNode.h"



class InterpreterVisitor : Visitor {
  public:
    void visit(AstNode &node);
    void visit(IfAstNode &node);
    void visit(DeclAstNode &node);
    void visit(AssignAstNode &node);
    void visit(ForAstNode &node);
    void visit(PrintAstNode &node);
    void visit(ReadAstNode &node);
    void visit(StatementsAstNode &node);
    void visit(ErrorAstNode &node);
    void visit(ExprAstNode &node);
    void visit(OpndAstNode &node);
  private:
  
    std::map<std::string, ExprValue> variables; 

    // use unique_ptr

    void initVar(Type type, std::string varId);
    void setVar(std::string varId, ExprValue val);
    ExprValue &getVar(std::string varId);
    ExprValue getVal(std::string varId);


    template<class T>
    ExprValue funcVisitor(T func, ExprValue a, ExprValue b) 
    {
      return std::visit( overloaded {
        [&](int arg1, int arg2) -> ExprValue {
          return func(arg1, arg2); 
        },
        [&](std::string arg1, std::string arg2) -> ExprValue {
          return func(arg1, arg2);
        },
        [&](bool arg1, bool arg2) -> ExprValue {
          return func(arg1, arg2); 
        },
        [&](auto arg1, auto arg2)  -> ExprValue {
          return false;
        }
      }, a, b);
    };

};


namespace Op {
  auto add = overloaded {
    [](auto a, auto b){return a+b;},
    [](bool a, bool b){return false;}
  };
  auto sub = overloaded {
    [](auto a, auto b){return false;},
    [](int a, int b){return a-b;}
  };
  auto mul = overloaded {
    [](auto a, auto b){return false;},
    [](int a, int b){return a*b;}
  };
  auto div = overloaded {
    [](auto a, auto b){return false;},
    [](int a, int b){return a/b;}
  };
  auto eq = [](auto a, auto b){return a == b;};
  auto less = [](auto a, auto b){return a < b;};  
  auto logAnd = overloaded {
    [](auto a, auto b){return false;},
    [](bool a, bool b){return a && b;}
  };
  auto logNot = overloaded {
    [](auto a){return false;},
    [](bool a){return !a;}
  };

}