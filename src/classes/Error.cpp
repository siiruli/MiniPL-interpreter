#include "Error.h"


void ErrorHandler::raiseError(Error error){
  errors.push_back(error);
  messager.printError(error);
}

bool ErrorHandler::hasErrors(){
  return !errors.empty();
}

void ErrorMessager::printError(Error error){
  output << error.type << " at " << error.programPosition << std::endl;
}