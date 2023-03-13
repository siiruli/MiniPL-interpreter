#include <iostream>
#include <fstream>
#include "MiniPL.h"


int main(int argc, char *argv[]){

  std::string filename;
  if(argc >= 2) filename = argv[1];
  else {
    std::cout << "Please provide a filename:\n";
    std::cin >> filename;
  } 
  MiniPL minipl(std::cin, std::cout);
  minipl.runFile(filename);
}