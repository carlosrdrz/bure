#pragma once

#include <string>

namespace bure {

enum level { debug, error, fatal };

class logger {
 public:
  static void log(level l, std::string format, va_list args);
  static void debug(std::string format...);
  static void error(std::string format...);
  static void fatal(std::string format...);
};

}  // namespace bure
