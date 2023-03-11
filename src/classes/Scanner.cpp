#include "Scanner.h"


#include <iostream>
#include <vector>

Scanner::Scanner(Program &program, ErrorHandler &handler) : 
  program(program), errorHandler(handler) {
} 

Token Scanner::getToken(){
  auto token = scanToken();
  while(!token.has_value()) token = scanToken();
  return token.value();
}

bool Scanner::isOperatorChar(char c){
  const char operatorChars[] = {
  '+', '-', '*', '/', '<', '=', '&', '!'
  };
  for(char a : operatorChars){
    if(c == a) return true;
  }
  return false;
}
bool Scanner::isPunctChar(char c){
  const char punctChars[] = {
  ';', ':', '.', '(', ')'
  };
  for(char a : punctChars){
    if(c == a) return true;
  }
  return false;
}
void Scanner::raiseError(ScanningErrorType errorType, std::string context){
  ScanningError error;

  error.contextSpan = Span{startOfToken, program.currentPosition()};
  error.span = Span{program.currentPosition(), program.currentPosition()};
  error.context = context;
  error.type = errorType;
  error.ch = program.currentChar();
  error.pos = program.currentPosition();

  errorHandler.raiseError(error);
}

std::optional<Token> Scanner::scanToken(){
  // Skip whitespace and comments
  while(auto c = program.currentChar()){
    // skip over comments
    if(*c == '/'){
      auto next = program.peekChar();
      if(next == '/' || next == '*') scanComment();
      else break;
    }
    // end if c is not whitespace
    else if(isspace(static_cast<unsigned char>(*c))) program.move();
    else break;
  }

  // scan a token  
  startOfToken = program.currentPosition();
  std::optional<TokenValue> token = std::nullopt;

  if(auto curChar = program.currentChar()){
    unsigned char c = static_cast<unsigned char>(*curChar);
    if(isalpha(c)){
      auto id = scanIdentifier();
      // if id is a keyword, return the keyword
      if(auto keyword = isKeyword(id.value())){
        token  = *keyword;
      }else{
        token = id;
      }
    }
    else if(isdigit(c)) token = scanInteger();
    else if(c == '"') token = scanString();
    else if(isPunctChar(c)) token = scanPunctuation();
    else if(isOperatorChar(c)) token = scanOperator();
    else {
      // Unexpected character
      std::string context = "start of token";
      raiseError(ScanningErrorType::UnexpChar, context);
    }
  }
  else{
    token = Delimiter::Eof;
  }

  Position endOfToken = program.currentPosition();
  program.move();

  if(auto value = token) {
    return Token{Span{startOfToken, endOfToken}, *value};
  }else {
    return {};
  }
}

void Scanner::scanComment(){
  char c1 = program.currentChar().value();
  program.move();
  char c2 = program.currentChar().value();
  program.move();

  if(c1 == '/' && c2 == '/'){
    while(
      program.currentChar().has_value() && 
      program.currentChar() != '\n'
    ){
      program.move();
    }
    program.move();
  }else if(c1 =='/' && c2 == '*'){
    int nestingLevel = 1;
    while(auto c = program.currentChar()){
      if(nestingLevel == 0) break;
      if(*c == '/' && program.peekChar() == '*') {
        ++nestingLevel;
        program.move();
      }
      else if(*c == '*' && program.peekChar() == '/'){
        --nestingLevel;
        program.move();
      }
      program.move();
    }
    if(nestingLevel != 0){
      raiseError(ScanningErrorType::Eof, "comment");
    }
  }
}

std::optional<Keyword> Scanner::isKeyword(std::string &ident){
  for(int i = 0; i<n_keywords; ++i) {
    if(ident == keywords[i]) 
      return static_cast<Keyword>(i);
  }
  return {};
}

std::optional<VarIdent> Scanner::scanIdentifier(){
  std::string ident = "";
  ident += program.currentChar().value();
  while(auto c = program.peekChar()){
    if(c != '_' && !isalnum(static_cast<unsigned char>(*c))) break;
    ident += *c;
    program.move();
  }
  return ident;
}

std::optional<Literal> Scanner::scanInteger(){
  std::string lexeme = "";
  lexeme += program.currentChar().value();
  while(auto c = program.peekChar()){
    if(!isdigit(static_cast<unsigned char>(*c))) break;
    lexeme += *c;
    program.move();
  }
  return Literal{std::stoi(lexeme)};
}

std::optional<Literal> Scanner::scanString(){
  std::string lexeme = "";

  if( auto c = program.currentChar(); *c == '"'){
    program.move();
  }else{
    // std::string msg = "Expected '\"' at start of string literal";
    raiseError(ScanningErrorType::UnexpChar, "start of string literal");
    return {};
  }

  while(program.currentChar() != '"'){
    if(!program.currentChar().has_value()){
      // error
      raiseError(ScanningErrorType::Eof, "string literal");
    }
    char c = program.currentChar().value();
    if(c == '\n'){
      // ERROR
      raiseError(ScanningErrorType::UnexpNewline, "string literal");
      break;
    }
    
    if(c == '\\'){
      // escape characters
      if(auto c = program.peekChar()){
        switch (*c)
        {
        case 'n': lexeme += '\n'; break;
        case 't': lexeme += '\t'; break;
        case '\n': break;
        default:
          lexeme += *c;
          break;
        }
        program.move();
      }
    }else{
      lexeme += c;
    }

    program.move();
  }
  return Literal{lexeme};
}

std::optional<Delimiter> Scanner::scanPunctuation(){
  std::optional<Delimiter> lexeme;
  if(auto c = program.currentChar()){
    switch (*c)
    {
    case ':':
      if(program.peekChar() == '=') {
        lexeme = Delimiter::Assign;
        program.move();
      }
      else lexeme = Delimiter::Colon;
      break;
    case ';':
      lexeme = Delimiter::Semicolon;
      break;
    case '(':
      lexeme = Delimiter::OpenParen;
      break;
    case ')':
      lexeme = Delimiter::ClosedParen;
      break;
    case '.':
      if(program.peekChar() == '.') {
        lexeme = Delimiter::Range;
        program.move();
      }
      else {
        // ERROR
        raiseError(ScanningErrorType::UnexpChar, "range operator");
        lexeme = Delimiter::Range;
      } 
      break; 
    default:
      lexeme = std::nullopt;
      break;
    }
  }
  return lexeme;
}

std::optional<Operator> Scanner::scanOperator(){
  std::optional<Operator> lexeme = std::nullopt;
  if(auto c = program.currentChar()){
    switch (*c)
    {
    case '+': lexeme = Operator::Add; break;
    case '-': lexeme = Operator::Sub; break;
    case '*': lexeme = Operator::Mul; break;
    case '/': lexeme = Operator::Div; break;
    case '<': lexeme = Operator::Less; break;
    case '=': lexeme = Operator::Equal; break;
    case '!': lexeme = Operator::Not; break;
    default:
      // error
      lexeme = std::nullopt;
      break;
    }
  }
  return lexeme;
}