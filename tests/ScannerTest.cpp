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
  std::string correctValue = "variable";
  Scanner scanner(program);
  auto tokenOpt = scanner.scanToken();
  EXPECT_TRUE(tokenOpt.has_value());
  Token token = tokenOpt.value();

  EXPECT_TRUE(std::holds_alternative<std::string>(token.value));
  EXPECT_EQ(std::get<std::string>(token.value), correctValue);
  
}
TEST_F(ScannerTest, scanKeyword) {
  std::vector<std::pair<std::string, Keyword>> tests = {
    {"var", Keyword::Var}, 
    {"for\n", Keyword::For}, 
    {"else", Keyword::Else}, 
    {"if ", Keyword::If}
  };
  for(auto [program, kw] : tests){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();
    EXPECT_TRUE(tokenOpt.has_value());
    Token token = tokenOpt.value();

    EXPECT_TRUE(std::holds_alternative<Keyword>(token.value));
    EXPECT_EQ(std::get<Keyword>(token.value), kw);
  }  
}
TEST_F(ScannerTest, scanString) {
  std::string program = 
    "\"This is in a string \" \
      This is not in a string";
  std::string correctValue = "This is in a string ";
  Scanner scanner(program);
  auto tokenOpt = scanner.scanToken();
  EXPECT_TRUE(tokenOpt.has_value());
  Token token = tokenOpt.value();

  EXPECT_TRUE(std::holds_alternative<std::string>(token.value));
  EXPECT_EQ(std::get<std::string>(token.value), correctValue);
  
}

TEST_F(ScannerTest, scanInt) {
  std::vector<std::pair<std::string, int>> tests = {
    {"10", 10}, 
    {"0 ", 0}, 
    {"503217103\n", 503217103}, 
    {"01401840;", 1401840}
  };
  for(auto [program, correct] : tests){
    Scanner scanner(program);
    auto tokenOpt = scanner.scanToken();
    EXPECT_TRUE(tokenOpt.has_value());
    Token token = tokenOpt.value();

    EXPECT_TRUE(std::holds_alternative<int>(token.value));
    EXPECT_EQ(std::get<int>(token.value), correct);
  }  
}