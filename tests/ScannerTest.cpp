#include "gtest/gtest.h"
#include "Scanner.h"

class ScannerTest :  
  // public ::testing::Test,
  public testing::TestWithParam<std::pair<Program, Token>> 
{
  public:

  protected:
    void SetUp() override {
    }
};
Token makeToken(uint start, uint end, TokenValue value){
  return Token{Position{0, start}, Position{0, end}, value};
}


std::ostream & operator<<(std::ostream &os, Token token){
  return os << "token"; 
}

TEST_P(ScannerTest, getToken) {
  auto [program, correctToken] = GetParam();
  ErrorHandler handler{program};
  Scanner scanner(program, handler);

  Token token = scanner.getToken();

  EXPECT_EQ(token.value, correctToken.value);
  EXPECT_EQ(token.span, correctToken.span);

}

INSTANTIATE_TEST_SUITE_P(
  scan_punctuation, 
  ScannerTest, 
  testing::Values(
    std::pair{Program{";"}, makeToken(0, 0, Delimiter::Semicolon)}, 
    std::pair{Program{".."}, makeToken(0, 1, Delimiter::Range)}, 
    std::pair{Program{" /* */ :=0\n"}, makeToken(7, 8, Delimiter::Assign)}, 
    std::pair{Program{")"}, makeToken(0, 0, Delimiter::ClosedParen)}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_comment, 
  ScannerTest, 
  testing::Values(
    std::pair{Program{"// comment \n"}, 
      Token{Position{1,0},Position{1,0}, Delimiter::Eof}},
    std::pair{Program{"/* this */"}, 
      makeToken(10, 10, Delimiter::Eof)},
    std::pair{Program{"/* /* nested comment */ */"}, 
      makeToken(26, 26, Delimiter::Eof)}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_identifier, 
  ScannerTest, 
  testing::Values(
    std::pair{Program{"x"}, makeToken(0, 0, "x")}, 
    std::pair{Program{"abc_24"}, makeToken(0, 5, "abc_24")}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_keyword, 
  ScannerTest, 
  testing::Values(
    std::pair{Program{"var"}, makeToken(0, 2, Keyword::Var)}, 
    std::pair{Program{" if "}, makeToken(1, 2, Keyword::If)}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_int, 
  ScannerTest, 
  testing::Values(
    std::pair{Program{"10"}, makeToken(0, 1, Literal{10})}, 
    std::pair{Program{"0;"}, makeToken(0, 0, Literal{0})}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_operator, 
  ScannerTest, 
  testing::Values(
    std::pair{Program{"+-"}, makeToken(0, 0, Operator::Add)},
    std::pair{Program{"!;"}, makeToken(0, 0, Operator::Not)},
    std::pair{Program{"/var"}, makeToken(0, 0, Operator::Div)}
  )
);
