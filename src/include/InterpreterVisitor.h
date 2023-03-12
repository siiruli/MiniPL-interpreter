#include <map>
#include "Util.h"
#include "Visitor.h"
#include "AstNode.h"
#include "Error.h"
/** @file */ 

struct RunTimeException : public std::exception {
  const char* type;
  RunTimeException(const char* type) : type(type) {}
  const char * what () const throw () {
    return type;
  }
};

class InterpreterVisitor : Visitor {
  public:
    InterpreterVisitor(std::istream &in, std::ostream &out, ErrorHandler &handler) :
      input(in), output(out), handler(handler) {}
    std::istream &input;
    std::ostream &output;
    ErrorHandler &handler;
    void visit(StatementNode &node);

    ExprValue visit(ExprNode &node);
    ExprValue visit(UnaryOp &node);
    ExprValue visit(BinaryOp &node);
    ExprValue visit(VarNode &node);
    

    void visit(IfAstNode &node);
    void visit(DeclAstNode &node);
    void visit(AssignAstNode &node);
    void visit(ForAstNode &node);
    void visit(PrintAstNode &node);
    void visit(ReadAstNode &node);
    void visit(StatementsAstNode &node);

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
          exit(1); return "TypeError";
        }
      }, a, b);
    };

};


namespace Op {
  auto add = overloaded {
    [](auto a, auto b){return a+b;},
    [](bool a, bool b){exit(1); return "TypeError";}
  };
  auto sub = overloaded {
    [](auto a, auto b){exit(1); return "TypeError";},
    [](int a, int b){return a-b;}
  };
  auto mul = overloaded {
    [](auto a, auto b){exit(1); return "TypeError";},
    [](int a, int b){return a*b;}
  };
  auto div = overloaded {
    [](auto a, auto b){exit(1); return "TypeError";},
    [](int a, int b){
      if(b == 0){
        throw RunTimeException("Division by zero");
      }
      return a/b;
    }
  };
  auto eq = [](auto a, auto b){return a == b;};
  auto less = [](auto a, auto b){return a < b;};  
  auto logAnd = overloaded {
    [](auto a, auto b){exit(1); return "TypeError";},
    [](bool a, bool b){return a && b;}
  };
  auto logNot = overloaded {
    [](auto a){exit(1); return "TypeError";},
    [](bool a){return !a;}
  };

}