#include <assert.h>
#include "Error.h"


bool ErrorHandler::hasErrors(){
  return !errors.empty();
}
uint ErrorHandler::errorNumber(){
  return errors.size();
}
void ErrorHandler::printErrors(Program &program, std::ostream &output){
  ErrorMessager messager(program, output);
  for(uint i=0; i<errors.size(); ++i){
    messager.printError(*errors[i]);
  }
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
  if(!span.isValid()) return;
  auto [start, end] = span;
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
    s = std::string(end.linePos+1, '-');
    output << lineStart() << s << std::endl;
  }

}

void ErrorMessager::printError(ErrorBase &error){
  output << error.description() << std::endl;
  auto startLine = std::min(error.contextSpan.start, error.span.end).lineNumber;
  auto endLine = std::max(error.span.end, error.contextSpan.end).lineNumber;
  auto span = error.span;
  if(startLine > endLine) return;

  if(startLine < span.start.lineNumber) 
    output << lineString(startLine);
  if(startLine+1 < span.start.lineNumber)
    output << lineSkipped();

  printSpan(span);

  if(endLine > 1 + span.end.lineNumber)
    output << lineSkipped();
  if(endLine > span.end.lineNumber) 
    output << lineString(endLine);


  output << std::endl;

}