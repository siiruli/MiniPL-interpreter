#include "Error.h"


void ErrorHandler::raiseError(Error error){
  errors.push_back(error);
}

bool ErrorHandler::hasErrors(){
  return !errors.empty();
}