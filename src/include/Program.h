#include <optional>
#include <ostream>
#pragma once
struct Position
{
  unsigned int charIndex;
  unsigned int lineNumber;
  unsigned int linePos;
  inline bool operator==(const Position &other) const {
    return charIndex == other.charIndex ;
      // && lineNumber == other.lineNumber;
  }
  inline bool operator<(const Position &other) const {
    return this->charIndex < other.charIndex;
  }

  // inline std::ostream & operator<<(std::ostream &os){
  //   return os << this.charIndex;
  // }
};
inline std::ostream & operator<<(std::ostream &os, Position pos){
  return os << pos.lineNumber << ":" << pos.linePos;
}

struct Span
{
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

class ProgramIterator {
  public:
    ProgramIterator(std::string &program);
    void move();    
    const std::optional<char> currentChar();
    const std::optional<char> peekChar();
    Position currentPosition();
  private:
    std::string &program;
    Position pos;
};