#include <iostream>
#include <fstream>
#include "MiniPL.h"


int main(int argc, char *argv[]){
  if(argc < 2) {
    std::cout << "Please provide a filename\n";
    return 0;
  }  
  std::string filename = argv[1];
  std::ifstream file(filename);
  std::string program;
  if(!file.is_open()){
    std::cout << "Unable to open file: " << filename << std::endl;
    return 0;
  }
  std::string line;
  while(std::getline(file, line)){
    program += line + "\n";
  }
  MiniPL minipl;
  minipl.run(program);
}