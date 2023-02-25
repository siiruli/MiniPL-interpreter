#include "gtest/gtest.h"
#include "Scanner.h"
class ScannerTest : public ::testing::Test {
  protected:
    void SetUp() override {
    }
};

TEST_F(ScannerTest, comment) {

  std::vector<std::string> programs = {
    "// this is a comment \n ",
    "/* this is also a comment */",
    "/* /* nested comment */ */"
  };
  for(std::string program : programs){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();

    EXPECT_TRUE(tokenOpt.has_value());
    EXPECT_EQ(
      std::get<Punctuation>(tokenOpt.value().value), 
      Punctuation::Eof
    );
  }
  
}
TEST_F(ScannerTest, space) {

  std::vector<std::string> programs = {
    " ",
    "\n \n ",
    "\t "
  };
  for(std::string program : programs){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();
    EXPECT_TRUE(tokenOpt.has_value());
    EXPECT_EQ(
      std::get<Punctuation>(tokenOpt.value().value), 
      Punctuation::Eof
    );
  }
  
}
TEST_F(ScannerTest, scanIdent) {
  std::string program = 
    "variable fpmf \n afoaeim ";
  TokenValue correctValue = "variable";
  Scanner scanner(program);
  auto tokenOpt = scanner.scanToken();
  EXPECT_TRUE(tokenOpt.has_value());
  Token token = tokenOpt.value();

  EXPECT_EQ(token.value, correctValue);
  
}
TEST_F(ScannerTest, scanKeyword) {
  std::vector<std::pair<std::string, Token>> tests = {
    {"var", Token{Position{0, 0}, Position{2,0}, Keyword::Var}}, 
    {"for\n", Token{Position{0, 0}, Position{2,0}, Keyword::For}}, 
    {"else", Token{Position{0, 0}, Position{3,0}, Keyword::Else}}, 
    {"if ", Token{Position{0, 0}, Position{1,0}, Keyword::If}}
  };
  for(auto [program, correctToken] : tests){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();
    
    EXPECT_TRUE(tokenOpt.has_value());
    Token token = tokenOpt.value();

    EXPECT_EQ(token.value, correctToken.value);
    EXPECT_EQ(token.startPos, correctToken.startPos);
    EXPECT_EQ(token.endPos, correctToken.endPos);
  }  
}
TEST_F(ScannerTest, scanString) {
  std::string program = 
    "\"This is in a string \" \
      This is not in a string";
  TokenValue correctValue = Literal{"This is in a string "};
  Scanner scanner(program);
  auto tokenOpt = scanner.scanToken();
  EXPECT_TRUE(tokenOpt.has_value());
  Token token = tokenOpt.value();

  EXPECT_EQ(token.value, correctValue);
  
}

TEST_F(ScannerTest, scanInt) {
  std::vector<std::pair<std::string, Token>> tests = {
    {"10", Token{Position{0, 0}, Position{1,0}, Literal{10}}}, 
    {"0\n", Token{Position{0, 0}, Position{0,0}, Literal{0}}}, 
    {" /* */ 480752710\n", Token{
      Position{7, 0}, Position{15,0}, Literal{480752710}
    }}, 
    {"// \n \n 642 ", Token{
      Position{7, 0}, Position{9,0}, Literal{642}
    }}
  };
  for(auto [program, correctToken] : tests){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();
    
    EXPECT_TRUE(tokenOpt.has_value());
    Token token = tokenOpt.value();

    EXPECT_EQ(token.value, correctToken.value);
    EXPECT_EQ(token.startPos, correctToken.startPos);
    EXPECT_EQ(token.endPos, correctToken.endPos);
  }  
}
TEST_F(ScannerTest, scanOp) {
  std::vector<std::pair<std::string, Token>> tests = {
    {"+-", Token{Position{0, 0}, Position{0,0}, Operator::Add}}, 
    {"!", Token{Position{0, 0}, Position{0,0}, Operator::Not}}, 
    {" /* */ *0\n", Token{
      Position{7, 0}, Position{7,0}, Operator::Mul
    }}, 
    {"/var", Token{
      Position{0, 0}, Position{0,0}, Operator::Div
    }}
  };
  for(auto [program, correctToken] : tests){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();
    
    EXPECT_TRUE(tokenOpt.has_value());
    Token token = tokenOpt.value();

    EXPECT_EQ(token.value, correctToken.value);
    EXPECT_EQ(token.startPos, correctToken.startPos);
    EXPECT_EQ(token.endPos, correctToken.endPos);
  }  
}

TEST_F(ScannerTest, scanPunct) {
  std::vector<std::pair<std::string, Token>> tests = {
    {";", Token{Position{0, 0}, Position{0,0}, Punctuation::Semicolon}}, 
    {"..", Token{Position{0, 0}, Position{1,0}, Punctuation::Range}}, 
    {" /* */ :=0\n", Token{
      Position{7, 0}, Position{8,0}, Punctuation::Assign
    }}, 
    {")", Token{
      Position{0, 0}, Position{0,0}, Punctuation::ClosedParen
    }}
  };
  for(auto [program, correctToken] : tests){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();
    
    EXPECT_TRUE(tokenOpt.has_value());
    Token token = tokenOpt.value();

    EXPECT_EQ(token.value, correctToken.value);
    EXPECT_EQ(token.startPos, correctToken.startPos);
    EXPECT_EQ(token.endPos, correctToken.endPos);
  }  
}