#include <string>
#include <variant>


typedef std::variant<int, bool, std::string> Value;

// struct Value {
//   Type type;
//   union {
//     int Int;
//     bool Bool;
//     std::string String;
//   } value;
// };