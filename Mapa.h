#ifndef __H_MAPA__
#define __H_MAPA__

#include <list>
#include <string>

class Mapa {
public:
	int mapa_ancho, mapa_alto;

	int *mapa;
	int *elementos;
	bool *pisable;

	Mapa(std::string archivo);
	~Mapa();

	bool comprobarTilePisable(int x, int y);
};

#endif
