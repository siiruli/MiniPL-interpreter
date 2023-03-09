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
  auto [endLine, endPos] = error.span.end;
  output << startLine+1 << ": " << program[startLine];
  if(endLine > startLine + 1) output << "...\n";
  if(endLine > startLine) output << endLine+1 << ": " << program[endLine];

  output << std::endl;

}