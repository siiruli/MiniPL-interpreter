#include <map>
#include "Util.h"
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

class Interpreter {
  public:
    Interpreter(std::istream &in, std::ostream &out, ErrorHandler &handler) :
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
          throw(RunTimeException("wrong types")); return "TypeError";
        }
      }, a, b);
    };

};


namespace Op {
  auto add = overloaded {
    [](auto a, auto b){return a+b;},
    [](bool a, bool b){throw(RunTimeException("wrong types")); return "TypeError";}
  };
  auto sub = overloaded {
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";},
    [](int a, int b){return a-b;}
  };
  auto mul = overloaded {
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";},
    [](int a, int b){return a*b;}
  };
  auto div = overloaded {
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";},
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
    [](bool a, bool b){return a && b;},
    [](auto a, auto b){throw(RunTimeException("wrong types")); return "TypeError";}
  };
  auto logNot = [](ExprValue val) {
    if(std::holds_alternative<bool>(val)){
      return !std::get<bool>(val);
    }else{
      throw(RunTimeException("wrong type"));
    }
  };

}