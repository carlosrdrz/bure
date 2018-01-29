#ifndef __H_JUEGO__
#define __H_JUEGO__

#include "Jugador.h"
#include "Mapa.h"

#include <list>
#include <string>

typedef struct {
	char *msg;
	Jugador *jg;
	int vida;
} Mensaje;

class Juego {
    public:
    	// Constructor y destructor
        Juego();
        ~Juego();

    	// Variables de estado
        bool abierto;
        bool jugando;
        bool logged;

        // Funciones sobre el cliente
        void cerrar();

        // Mapas
        Mapa *mapaActual;
        void cambiarMapa(std::string archivo);

        // Jugadores
        void nuevoJugador(Jugador *pl);
        void quitarJugador(Jugador *pl);
        int jugadores();
        Jugador *getJugadorByIndex(int num);
        Jugador *getJugadorByNombre(string nombre);

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
        list<Jugador*> demas;
        list<Mensaje*> mensajes;
};
#endif
