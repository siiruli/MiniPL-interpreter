#include "gtest/gtest.h"
#include "AstNode.h"
#include "Interpreter.h"

class VisitorTest :  
  public ::testing::Test
  // public testing::TestWithParam<std::pair<std::string, Token>> 
{
  public:
    
  protected:
    void SetUp() override {

    }
    std::stringstream input, output;
    ErrorHandler handler;
    Interpreter visitor{input, output, handler};
};

TEST_F(VisitorTest, operandTest){
  // OpndAstNode node;
  // node.operand = Literal{5};
  // ExprValue correct = 5;
  // visitor.visit(node);
  // EXPECT_EQ(node.value, correct);
}

