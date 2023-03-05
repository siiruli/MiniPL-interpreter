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
    std::stringstream in, out;
    MiniPL miniPl{in, out};
    
};

TEST_F(CompilerTest, test1) {
  std::string program = 
    "var X : int := 4 + (6 * 2);\n"
    "print X;";

  miniPl.run(program);
  std::string outputStr = out.str();
  EXPECT_EQ(outputStr, "16");
}

TEST_F(CompilerTest, test2) {
  std::string program = 
    "var X : int := 4 - (6 / 2);\n"
    "print X;";

  miniPl.run(program);
  std::string outputStr = out.str();
  EXPECT_EQ(outputStr, "1");
}