#include <assert.h>
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


std::string ErrorMessager::lineStart(uint lineNumber){
  std::string number = std::to_string(lineNumber+1);
  return std::string(lineStartLength()-2-number.size(), ' ') + number + " |";
}
std::string ErrorMessager::lineString(uint lineNumber){
  if(lineNumber >= program.size()) return lineStart(lineNumber) + "\n";
  return lineStart(lineNumber) + program[lineNumber];
}

std::string ErrorMessager::lineStart(){
  return std::string(lineStartLength()-1, ' ') + "|";
}
std::string ErrorMessager::lineSkipped(){
  return std::string(lineStartLength()-2, ' ') + "...\n";
}
void ErrorMessager::printSpan(Span span){
  auto [start, end] = span;
  assert(start <= end);
  output << lineString(start.lineNumber);
  int n = end.lineNumber == start.lineNumber ? 
    end.linePos - start.linePos : 
    program[start.lineNumber].size()-start.linePos-2;
  std::string s = std::string(start.linePos, ' ') + "^" + std::string(n, '-');
  output << lineStart() << s << std::endl;

  if(end.lineNumber > 1+start.lineNumber){
    output << lineSkipped();
  }
  if(end.lineNumber > start.lineNumber){
    output << lineString(end.lineNumber);
    s = std::string(end.linePos, '-');
    output << lineStart() << s << std::endl;
  }

}

void ErrorMessager::printError(ErrorBase &error){
  output << error.description() << std::endl;
  auto startLine = std::min(error.contextSpan.start, error.contextSpan.end).lineNumber;
  auto endLine = std::max(error.span.start, error.contextSpan.end).lineNumber;
  auto span = error.span;


  if(startLine < span.start.lineNumber) 
    output << lineStart(startLine) << program[startLine];
  if(startLine+1 < span.start.lineNumber)
    output << lineSkipped();

  printSpan(span);

  if(endLine > 1 + span.end.lineNumber)
    output << lineSkipped();
  if(endLine > span.end.lineNumber) 
    output << lineStart(endLine) << program[endLine];


  output << std::endl;

}