#include <bits/stdc++.h>

struct Token {
  int lineNumber;
  int type;
  int value;
};

class Scanner {
  public:
    Token getToken();
  private: 

    char getChar();
    
    std::string scanString();
    void scanComment();
    

};