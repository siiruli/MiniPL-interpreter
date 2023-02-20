#include "Span.h"

enum class ErrorType {EOF};

struct Error {
  Span programPosition;
  ErrorType type;
};


