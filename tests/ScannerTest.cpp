#include "gtest/gtest.h"
#include "Scanner.h"

class ScannerTest :  
  // public ::testing::Test,
  public testing::TestWithParam<std::pair<std::string, Token>> 
{
  public:

  protected:
    void SetUp() override {
    }
};
Token makeToken(uint start, uint end, TokenValue value){
  return Token{Position{start, 0}, Position{end, 0}, value};
}

std::ostream & operator<<(std::ostream &os, Position pos){
  return os << "pos: " << pos.charIndex;
}
std::ostream & operator<<(std::ostream &os, Span span){
  return os << "start: " << span.start.charIndex 
    << ", end: " << span.end.charIndex;
}
std::ostream & operator<<(std::ostream &os, Token token){
  return os << "token"; 
}

TEST_P(ScannerTest, getToken) {
  auto [program, correctToken] = GetParam();
  ErrorHandler handler;
  Scanner scanner(program, handler);

  Token token = scanner.getToken();

  EXPECT_EQ(token.value, correctToken.value);
  EXPECT_EQ(token.span, correctToken.span);

}

INSTANTIATE_TEST_SUITE_P(
  scan_punctuation, 
  ScannerTest, 
  testing::Values(
    std::pair{";", makeToken(0, 0, Delimiter::Semicolon)}, 
    std::pair{"..", makeToken(0, 1, Delimiter::Range)}, 
    std::pair{" /* */ :=0\n", makeToken(7, 8, Delimiter::Assign)}, 
    std::pair{")", makeToken(0, 0, Delimiter::ClosedParen)}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_comment, 
  ScannerTest, 
  testing::Values(
    std::pair{"// comment \n ", makeToken(13, 13, Delimiter::Eof)},
    std::pair{"/* this */", 
      makeToken(10, 10, Delimiter::Eof)},
    std::pair{"/* /* nested comment */ */", 
      makeToken(26, 26, Delimiter::Eof)}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_identifier, 
  ScannerTest, 
  testing::Values(
    std::pair{"x", makeToken(0, 0, "x")}, 
    std::pair{"abc_24", makeToken(0, 5, "abc_24")}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_keyword, 
  ScannerTest, 
  testing::Values(
    std::pair{"var", makeToken(0, 2, Keyword::Var)}, 
    std::pair{" if ", makeToken(1, 2, Keyword::If)}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_int, 
  ScannerTest, 
  testing::Values(
    std::pair{"10", makeToken(0, 1, Literal{10})}, 
    std::pair{"0;", makeToken(0, 0, Literal{0})}
  )
);

INSTANTIATE_TEST_SUITE_P(
  scan_operator, 
  ScannerTest, 
  testing::Values(
    std::pair{"+-", makeToken(0, 0, Operator::Add)},
    std::pair{"!;", makeToken(0, 0, Operator::Not)},
    std::pair{"/var", makeToken(0, 0, Operator::Div)}
  )
);
