#include "gtest/gtest.h"
#include "Program.h"
class ProgramTest : public ::testing::Test {
  protected:
    void SetUp() override {
    }
};

TEST_F(ProgramTest, programIterator) {
  std::string program = "a\nb";
  ProgramIterator it(program);

  EXPECT_EQ(it.currentPosition().charIndex, 0);
  EXPECT_EQ(it.currentPosition().lineNumber, 0);
  EXPECT_EQ(it.currentChar(), 'a');
  EXPECT_EQ(it.peekChar(), '\n');
  
  it.move();
  EXPECT_EQ(it.currentPosition().charIndex, 1);
  EXPECT_EQ(it.currentPosition().lineNumber, 0);
  EXPECT_EQ(it.currentChar(), '\n');
  EXPECT_EQ(it.peekChar(), 'b');
  
  it.move();
  EXPECT_EQ(it.currentPosition().charIndex, 2);
  EXPECT_EQ(it.currentPosition().lineNumber, 1);
  EXPECT_EQ(it.currentChar(), 'b');
  EXPECT_FALSE(it.peekChar().has_value());
  
}
