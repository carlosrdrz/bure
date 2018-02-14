#ifndef __H_GAME__
#define __H_GAME__

#include "player.h"
#include "game_map.h"
#include "graphics.h"
#include "player.h"
#include "error.h"

#include <list>
#include <string>

typedef struct {
	char *msg;
	player *jg;
	int vida;
} Mensaje;

class game {
    public:
    	// Constructor y destructor
        game();
        ~game();

    	// Variables de estado
        bool abierto;
        bool playing;
        bool logged;

        // Funciones sobre el cliente
        void cerrar();

        // Mapas
        game_map *currentMap;
        void cambiarMapa(std::string archivo);

        // Jugadores
        void newPlayer(player *pl);
        void removePlayer(player *pl);
        int jugadores();
        player *getPlayerByIndex(int num);
        player *getPlayerByNombre(string nombre);

        // Loop de mensajes
        bool loopMsgActivo;
        void addMensaje(Mensaje *msg);
        void quitMensaje(Mensaje *msg);
        void MensajesLoop();

        // Animaciones
        void nextAnimationFrame();

   private:
        list<player*> demas;
        list<Mensaje*> mensajes;
};
#endif
