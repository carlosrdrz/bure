#ifndef __H_JUEGO__
#define __H_JUEGO__

#include "player.h"
#include "map.h"

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
        map *currentMap;
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

        // Banderas de threads
        bool princp;
        bool princp_parado;

   private:
        list<player*> demas;
        list<Mensaje*> mensajes;
};
#endif
