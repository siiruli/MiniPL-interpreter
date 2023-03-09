#include "gtest/gtest.h"
#include "Parser.h"

typedef std::vector<Token> TokenList;
typedef std::vector<TokenValue> ValueList;

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
    Token eof = Token{Span{}, Delimiter::Eof};


};

class ParserTest :  
  public ::testing::Test
  // public testing::TestWithParam<std::pair<std::string, Token>> 
{
  public:
    Token makeToken(TokenValue value){
      return Token{Span{}, value};
    }
    TokenList makeTokens(ValueList &vals){
      TokenList tokens(vals.size());
      for(uint i=0; i<vals.size(); ++i) tokens[i] = makeToken(vals[i]);
      return tokens;
    }
  protected:
    ErrorHandler handler;
    void SetUp() override {
    }
};

TEST_F(ParserTest, assignment) {
  VarIdent x = "x";
  ValueList list = {
    x, Delimiter::Assign, Literal{0}, Delimiter::Semicolon
  };
  TestIterator it(makeTokens(list));

  Parser parser(it, handler);
  AstNode node = parser.program();
  ASSERT_TRUE(std::holds_alternative<StatementsAstNode>(node));
  auto &stmts = std::get<StatementsAstNode>(node);

  ASSERT_EQ(stmts.statements.size(), 1);
  auto &stmt = stmts.statements[0];
  ASSERT_TRUE(std::holds_alternative<AssignAstNode>(stmt));
  auto &assign = std::get<AssignAstNode>(stmt);
  
  EXPECT_EQ(assign.varId, x);
  // EXPECT_EQ(assign.expr.opnd1.get()->operand, Operand{Literal{0}});
}
