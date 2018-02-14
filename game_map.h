#ifndef __H_GAME_MAP__
#define __H_GAME_MAP__

#include "error.h"

#include <libxml++/libxml++.h>

#include <iostream>
#include <fstream>
#include <list>
#include <string>

class game_map {
public:
	int width, height;

	int *mapa;
	int *elementos;
	bool *pisable;

	explicit game_map(std::string basePath, std::string archivo);
	~game_map();

	bool comprobarTilePisable(int x, int y);
};

#endif
