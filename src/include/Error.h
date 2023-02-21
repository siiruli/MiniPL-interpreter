#include "Program.h"

enum class ErrorType {Eof};
enum class ErrorSource {Scanner, Parser, Interpreter};
struct Error {
  Span programPosition;
  ErrorType type;
  ErrorSource source;
};


