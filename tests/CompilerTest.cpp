#include "gtest/gtest.h"
#include "MiniPL.h"


class CompilerTest :  
  public ::testing::Test
  // public testing::TestWithParam<std::pair<std::string, Token>> 
{
  public:
    
  protected:
    void SetUp() override {
    }
};

TEST_F(CompilerTest, test1) {
  MiniPL miniPl;
  std::string program = 
    "var X : int := 4 + (6 * 2);\n"
    "print X;";

  miniPl.run(program);
}

TEST_F(CompilerTest, test2) {
  MiniPL miniPl;
  std::string program = 
    "var X : int := 4 - (6 / 2);\n"
    "print X;";

  miniPl.run(program);
}