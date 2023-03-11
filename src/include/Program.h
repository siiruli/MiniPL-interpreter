#include <optional>
#include <ostream>
#include <vector>
#pragma once

struct Position
{
  inline Position() : lineNumber(0), linePos(0) {}
  
  inline Position(uint lineNumber, uint linePos) : 
    lineNumber(lineNumber), linePos(linePos) {}

  unsigned int lineNumber;
  unsigned int linePos;
  inline bool operator==(const Position &other) const {
    return lineNumber == other.lineNumber
            && linePos == other.linePos ;
  }
  inline bool operator<(const Position &other) const {
    return std::pair{lineNumber, linePos} < 
           std::pair{other.lineNumber, other.linePos};
  }
  inline bool operator<=(const Position &other) const {
    return std::pair{lineNumber, linePos} <=
           std::pair{other.lineNumber, other.linePos};
  }
  // inline std::ostream & operator<<(std::ostream &os){
  //   return os << this.charIndex;
  // }
};
inline std::ostream & operator<<(std::ostream &os, Position pos){
  return os << pos.lineNumber+1 << ":" << pos.linePos+1;
}

struct Span
{
  Span() : start(-1, -1), end(0,0) {}
  Span(Position start, Position end) : start(start), end(end) {}
  Span(uint a, uint b, uint c, uint d) : start(a, b), end(c,d) {}

  Position start, end;
  inline bool operator==(const Span &other) const {
    return this->start == other.start && this->end == other.end;
  }  
  inline Span operator+(const Span&other) const {
    return Span{
      std::min(this->start, other.start), 
      std::max(this->start, other.end)
    };
  }  
  inline void operator+=(const Span&other) {
    this->start = std::min(this->start, other.start);
    this->end = std::max(this->start, other.end);
  }
};

inline std::ostream & operator<<(std::ostream &os, Span span){
  return os << span.start << "-" << span.end;
}

typedef std::vector<std::string> Program;

class ProgramIterator {
  public:
    ProgramIterator(Program &program);
    void move();    
    const std::optional<char> currentChar();
    const std::optional<char> peekChar();
    Position currentPosition();
  private:
    const Position nextPos(Position pos);
    const std::optional<char> getChar(Position pos);
    Program &program;
    Position pos;
};
