#include <optional>

struct Position
{
  unsigned int charIndex;
  unsigned int lineNumber;
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