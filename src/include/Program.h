#include <optional>

struct Position
{
  unsigned int charIndex;
  unsigned int lineNumber;
  inline bool operator==(const Position &other) const {
    return charIndex == other.charIndex ;
      // && lineNumber == other.lineNumber;
  }
};


struct Span
{
  Position start, end;  
};

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