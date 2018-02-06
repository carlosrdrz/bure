#ifndef __H_MAPA__
#define __H_MAPA__

#include <list>
#include <string>

class map {
public:
	int width, height;

	int *mapa;
	int *elementos;
	bool *pisable;

	map(std::string archivo);
	~map();

	bool comprobarTilePisable(int x, int y);
};

#endif
