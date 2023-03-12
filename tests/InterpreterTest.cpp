#include "gtest/gtest.h"
#include "MiniPL.h"


class SampleTest :  
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
TEST_P(SampleTest, program) {
  auto [program, input, output] = GetParam();

  in << input;
  miniPl.run(program);
  EXPECT_EQ(out.str(), output);
}

INSTANTIATE_TEST_SUITE_P(
  arithmetic, 
  SampleTest, 
  testing::Values(
    std::tuple{
      Program{"var X : int := ((1 - (6 / 2)));\n",
      "print X;"},
      "", "-2"
    },
    std::tuple{
      Program{"var X : int := 4 + (6 * 2);\n",
      "print X;"},
      "", "16"
    }
  )
);

INSTANTIATE_TEST_SUITE_P(
  if_statement, 
  SampleTest, 
  testing::Values(
    std::tuple{
      Program{"var X : int;\n",
      "read X; \n",
      "if X = 1 do print \"one\";\n ",
      "else print \"not one\"; print \".\";\n",
      "end if;"},
      "1 ", "one"
    },
    std::tuple{
      Program{"var X : int;\n",
      "read X; \n",
      "if X = 1 do print \"one\";\n ",
      "else print \"not one\"; print \".\";"
      "end if;"},
      "2 ", "not one."
    }
  )
);
INSTANTIATE_TEST_SUITE_P(
  for_statement, 
  SampleTest, 
  testing::Values(
    std::tuple{
      Program{"var X : int; // comment\n",
      "for X in 0-2..1*2 do \n",
      "print X; print \" \"; end for;"},
      "nothing", "-2 -1 0 1 2 "
    }
  )
);