#include "Program.h"

ProgramIterator::ProgramIterator(Program &program) :
  program(program) 
{
  pos = Position{0, 0};
}

void ProgramIterator::move(){
  pos = nextPos(pos);
}

const std::optional<char> ProgramIterator::currentChar(){
  return getChar(pos);
}

const std::optional<char> ProgramIterator::peekChar(){
  return getChar(nextPos(pos));
}

Position ProgramIterator::currentPosition(){
  return pos;
}

const Position ProgramIterator::nextPos(Position pos){
  uint lineNumber = pos.lineNumber;
  uint linePos = pos.linePos;
  if(getChar(pos) == '\n') {
    ++lineNumber; linePos = 0;
  }else if(getChar(pos).has_value()) {
    ++linePos;
  }
  return Position{lineNumber, linePos};
}
const std::optional<char> ProgramIterator::getChar(Position pos){
  if(pos.lineNumber < program.size() 
    && pos.linePos < program[pos.lineNumber].size()){
    return program[pos.lineNumber][pos.linePos];
  }else return {};
}