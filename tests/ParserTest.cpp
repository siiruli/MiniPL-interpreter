#include "gtest/gtest.h"
#include "Parser.h"

typedef std::vector<Token> TokenList;

class TestIterator : public TokenIterator {
  public: 
    TestIterator(std::vector<Token> list){
      tokenList = list;
      tokenList.push_back(eof);
    }
    virtual Token currentToken() override {
      return tokenList[idx];
    }
    virtual Token nextToken() override {
      if(idx+1 == tokenList.size()) return eof;
      return tokenList[idx++];
    }
  private:
    std::vector<Token> tokenList;
    uint idx = 0;
    Token token;
    Token eof = Token{0, 0,0,0, Delimiter::Eof};

};

class ParserTest :  
  public ::testing::Test
  // public testing::TestWithParam<std::pair<std::string, Token>> 
{
  public:
    Token makeToken(TokenValue value){
      return Token{0,0,0,0, value};
    }
  protected:
    void SetUp() override {
    }
};

TEST_F(ParserTest, statements) {
  TokenList list = {};
  TestIterator it(list);
  Parser parser(it);
  AstNode node = parser.program();
  EXPECT_TRUE(std::holds_alternative<StatementsAstNode>(node));
  
}
