
#include "gtest/gtest.h"
#include "MiniPL.h"


class SemAnTest :  
  // public ::testing::Test
  public testing::TestWithParam<std::tuple<Program, std::string, std::string>> 
{
  public:
    
  protected:
    void SetUp() override {
    }
    std::stringstream in, out;
    MiniPL miniPl{in, out};
    
};
TEST_P(SemAnTest, program) {
  auto [program, input, output] = GetParam();

  in << input;
  miniPl.run(program);
  // EXPECT_EQ(out.str(), output);
}

INSTANTIATE_TEST_SUITE_P(
  typechecker, 
  SemAnTest, 
  testing::Values(
    std::tuple{
      Program{
        "var x : int := \"8\"; // int can't be initialized to string \n",
        " var y : string := 5 + \"hello\"; // Can't add string to int\n",
        " var z : bool := 1 = 1; // This is correct\n",
        "print x;\n",
        "print \"\\n\";\n",
        "print y;\n",
        "print \"\\n\";\n",
        "print (!z) * z; // can't multiply booleans\n"
      },
      "", ""
    }
  )
);

