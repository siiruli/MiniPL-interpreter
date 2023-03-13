#include "gtest/gtest.h"
#include "MiniPL.h"


class MiniPlTest :  
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
TEST_P(MiniPlTest, program) {
  auto [program, input, output] = GetParam();

  in << input;
  miniPl.run(program);
  EXPECT_EQ(out.str(), output);
}

INSTANTIATE_TEST_SUITE_P(
  arithmetic, 
  MiniPlTest, 
  testing::Values(
    std::tuple{
      Program{"var X : int := ((1 - (6 / 2)));\n",
      "print X;\n"},
      "", "-2"
    },
    std::tuple{
      Program{"var X : int := 4 + (6 * 2);\n",
      "print X;"},
      "", "16"
    },
    std::tuple{
      Program{"var X : bool := !!(1+3 = 4/4*4);\n",
      "print X;\n",
      "print !!!!!X;\n"},
      "", "10"
    },
    std::tuple{
      Program{"var X : string := \"essr\" + \"a\";\n",
      "print X + \"\\n\";\n",
      "print X + X;\n"},
      "", "essra\nessraessra"
    },
    std::tuple{
      Program{"var X : bool := \"aa\" < \"ab\" = 1 < 2;\n",
      "print X;\n",
      "print \"\\n\";\n",
      "print X & X;\n"},
      "", "1\n1"
    }
  )
);

INSTANTIATE_TEST_SUITE_P(
  if_statement, 
  MiniPlTest, 
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
  MiniPlTest, 
  testing::Values(
    std::tuple{
      Program{"var X : int; // comment\n",
      "for X in 0-2..1*2 do \n",
      "print X; print \" \"; end for;"},
      "nothing", "-2 -1 0 1 2 "
    }
  )
);