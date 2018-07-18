#pragma once

#include <string>

namespace bure {

class logger {
 public:
  static void log(std::string error, int import);
};

}  // namespace bure
