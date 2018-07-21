#include "logger.h"

#include <stdio.h>
#include <ctime>
#include <cstdarg>
#include <iomanip>
#include <iostream>

namespace bure {

void logger::log(level l, std::string format, va_list args) {
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  std::cout << "[" << std::put_time(&tm, "%F %T") << "]";

  switch (l) {
    case level::debug:
      std::cout << " [DEBUG] ";
      break;
    case level::error:
      std::cout << " [ERROR] ";
      break;
  }

  vfprintf(stdout, format.c_str(), args);
  std::cout << std::endl;
}

void logger::debug(std::string format...) {
  va_list args;
  va_start(args, format);
  log(level::debug, format, args);
  va_end(args);
}

void logger::error(std::string format...) {
  va_list args;
  va_start(args, format);
  log(level::error, format, args);
  va_end(args);
}

}  // namespace bure
