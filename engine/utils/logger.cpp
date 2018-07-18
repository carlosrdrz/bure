#include "logger.h"

#include <time.h>
#include <fstream>

namespace bure {

void logger::log(std::string error, int import) {
  std::ofstream salida("errores.txt", std::ios_base::app);

  time_t fecha_hora;
  time(&fecha_hora);

  char buf[50];
  ctime_r(&fecha_hora, buf);
  salida << buf;
  salida << " - ";

  switch (import) {
    case 0:
      salida << "debug: ";
      break;
    case 1:
      salida << "warn: ";
      break;
    case 2:
      salida << "error: ";
      break;
    case 3:
      salida << "critical: ";
      break;
  }

  salida << error;
  salida << "\n";

  salida.close();
}

}  // namespace bure
