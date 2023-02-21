#include "gtest/gtest.h"
#include "Scanner.h"
class ScannerTest : public ::testing::Test {
  protected:
    void SetUp() override {
    }
};

TEST_F(ScannerTest, scanString) {
  std::string program = 
    "\"This is in a string \\n over two lines\" \
      This is not in a string";
  std::string correctValue = "This is in a string \n over two lines";
  Scanner scanner(program);
  auto tokenOpt = scanner.getToken();
  EXPECT_TRUE(tokenOpt.has_value());
  Token token = tokenOpt.value();

  EXPECT_TRUE(std::holds_alternative<std::string>(token.value));
  EXPECT_EQ(std::get<std::string>(token.value), correctValue);
  
}