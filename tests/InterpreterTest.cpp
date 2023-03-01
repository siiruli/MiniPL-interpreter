#include "gtest/gtest.h"
#include "AstNode.h"
#include "InterpreterVisitor.h"

class InterpreterTest :  
  public ::testing::Test
  // public testing::TestWithParam<std::pair<std::string, Token>> 
{
  public:
    
  protected:
    void SetUp() override {

    }
    InterpreterVisitor visitor;
};

TEST_F(InterpreterTest, operandTest){
  OpndAstNode node;
  node.operand = Literal{5};
  ExprValue correct = 5;
  visitor.visit(&node);
  EXPECT_EQ(node.value, correct);
}

