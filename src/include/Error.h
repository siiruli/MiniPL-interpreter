#include <variant>
#include <vector>
#include "Program.h"

enum class ScanningError {UnexpChar, Eof};

typedef std::variant<ScanningError> ErrorType;

struct Error {
  Span programPosition;
  ErrorType type;
  std::string info;
};

class ErrorHandler {
  public:
    void raiseError(Error error);
    bool hasErrors();
  private:
    std::vector<Error> errors;
};

class ErrorMessager {
  public:
    void printError(Error error);
};