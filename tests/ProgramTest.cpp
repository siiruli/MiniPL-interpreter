#include "gtest/gtest.h"
#include "Program.h"
class ProgramTest : public ::testing::Test {
  protected:
    void SetUp() override {
    }
};

TEST_F(ProgramTest, programIterator) {
  Program program = {"a\n", "b"};
  ProgramIterator it(program);

  EXPECT_EQ(it.currentPosition().linePos, 0);
  EXPECT_EQ(it.currentPosition().lineNumber, 0);
  EXPECT_EQ(it.currentChar(), 'a');
  EXPECT_EQ(it.peekChar(), '\n');
  
  it.move();
  EXPECT_EQ(it.currentPosition().linePos, 1);
  EXPECT_EQ(it.currentPosition().lineNumber, 0);
  EXPECT_EQ(it.currentChar(), '\n');
  EXPECT_EQ(it.peekChar(), 'b');
  
  it.move();
  EXPECT_EQ(it.currentPosition().linePos, 0);
  EXPECT_EQ(it.currentPosition().lineNumber, 1);
  EXPECT_EQ(it.currentChar(), 'b');
  EXPECT_FALSE(it.peekChar().has_value());


  it.move();
  EXPECT_EQ(it.currentPosition().linePos, 1);
  EXPECT_EQ(it.currentPosition().lineNumber, 1);
  EXPECT_FALSE(it.currentChar().has_value());
  EXPECT_FALSE(it.peekChar().has_value());
  
}
