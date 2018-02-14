#ifndef __H_GAME__
#define __H_GAME__

#include "player.h"
#include "game_map.h"
#include "graphics.h"
#include "player.h"
#include "error.h"

#include <list>
#include <string>

class game {
    public:
    	// Constructor y destructor
        game();

    	// Variables de estado
        bool abierto;
        bool playing;
        bool logged;

        // Funciones sobre el cliente
        void cerrar();

        // Mapas
        game_map *currentMap;
        void cambiarMapa(std::string archivo);

        // Animaciones
        void nextAnimationFrame();
};
#endif
