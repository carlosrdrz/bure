#include "logger.h"

#include <stdio.h>

#include <cstdarg>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "backward.hpp"

using namespace backward;

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
    case level::fatal:
      std::cout << " [FATAL] ";
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

void logger::fatal(std::string format...) {
  va_list args;
  va_start(args, format);
  log(level::fatal, format, args);
  va_end(args);

  Printer p;
  StackTrace st;
  st.load_here(32);
  st.skip_n_firsts(3);
  p.print(st);
  exit(1);
}

}  // namespace bure
