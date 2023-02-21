#include "Program.h"

ProgramIterator::ProgramIterator(std::string &program) :
  program(program) 
{
  pos = Position{0, 0};
}

void ProgramIterator::move(){
  if(currentChar() == '\n') ++pos.lineNumber;
  ++pos.charIndex;
}

const std::optional<char> ProgramIterator::currentChar(){
  if(pos.charIndex < program.size()) 
    return program[pos.charIndex];
  else return {};
}

const std::optional<char> ProgramIterator::peekChar(){
  if(pos.charIndex+1 < program.size()) 
    return program[pos.charIndex+1];
  else return {};
}

Position ProgramIterator::currentPosition(){
  return pos;
}