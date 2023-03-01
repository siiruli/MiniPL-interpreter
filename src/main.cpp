#include "MiniPL.h"


int main(){
  std::string program = 
  "print \"Give a number\";\n"
  "var n : int;\n"
  "read n;\n"
  "var v : int := 1;\n"
  "var i : int;\n"
  "for i in 1..n do\n"
  "v := v * i;\n"
  "end for;\n"
  "print \"The result is: \";\n"
  "print v;\n";

  MiniPL minipl;
  minipl.run(program);

  return 0;
}