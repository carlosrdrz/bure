#include "error.h"

#include <fstream>
#include <time.h>

void Error::Log(std::string error, int import)
{
	std::ofstream salida ("errores.txt", std::ios_base::app);

	time_t fecha_hora;
	time(&fecha_hora);

	salida << ctime(&fecha_hora);
	salida << " - ";

	switch(import) {
		case 0:
			salida << "Nota: ";
			break;
		case 1:
			salida << "Advertencia: ";
			break;
		case 2:
			salida << "Error: ";
			break;
		case 3:
			salida << "!!Error critico!!: ";
			break;
	}

	salida << error;
	salida << "\n";

	salida.close();
}

void Error::Log(int error, int import) {
	std::ofstream salida ("errores.txt", std::ios_base::app);

	time_t fecha_hora;
	time(&fecha_hora);

	salida << ctime(&fecha_hora);
	salida << " - ";

	switch(import) {
		case 0:
			salida << "Nota: ";
			break;
		case 1:
			salida << "Advertencia: ";
			break;
		case 2:
			salida << "Error: ";
			break;
		case 3:
			salida << "!!Error critico!!: ";
			break;
	}

	salida << error;
	salida << "\n";

	salida.close();
}
