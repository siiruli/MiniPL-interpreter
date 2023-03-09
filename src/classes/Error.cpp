#include "Error.h"


void ErrorHandler::raiseError(Error error){
  errors.push_back(error);
  messager.printError(error);
}

bool ErrorHandler::hasErrors(){
  return !errors.empty();
}

void ErrorMessager::printError(Error &error){
  ErrorBase &base = std::visit([](auto &arg) -> ErrorBase& {
    return static_cast<ErrorBase&>(arg);
  }, error);
  printError(base);
}


void ErrorMessager::printError(ErrorBase &error){
  output << error.description() << std::endl;
  auto [startLine, startPos] = error.span.start;
  output << startLine+1 << ": " << program[startLine];

}