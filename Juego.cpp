#include "Juego.h"
#include "Mapa.h"
#include "Graficos.h"
#include "Jugador.h"
#include "Error.h"

#include <string>

extern Graficos *pantalla;
extern Jugador *player;

Juego::Juego()
{
    mapaActual = NULL;
    abierto = true;
    jugando = false;
    loopMsgActivo = true;
    logged = false;
    princp = true;
    princp_parado = false;
}

Juego::~Juego()
{
	demas.clear();
	mensajes.clear();
}

void Juego::cerrar()
{
    abierto = false;
    jugando = false;
}

void Juego::cambiarMapa(std::string archivo)
{
	// Paramos thread principal de dibujo
	princp = false;
	while(!princp_parado);
	delete mapaActual;
	mapaActual = new Mapa(archivo);
	princp = true;
}

void Juego::nuevoJugador(Jugador *pl)
{
    pl->index = (int)demas.size();
    demas.push_back(pl);
}

void Juego::quitarJugador(Jugador *pl)
{
	loopMsgActivo = false;
	if(mensajes.size() > 0) {
		std::list<Mensaje*>::iterator i;
		for(int m = 0; m <= pl->mensajesMostrados; m++) {
			for(i = mensajes.begin(); i != mensajes.end(); i++) {
				if((*i)->jg == pl) {
					mensajes.erase(i);
					break;
				}
			}
		}
	}

	demas.remove(pl);
	loopMsgActivo = true;
}

int Juego::jugadores()
{
    return (int)demas.size();
}

Jugador *Juego::getJugadorByIndex(int num)
{
    if(demas.size() > 0) {
		std::list<Jugador*>::iterator i;
		for(i = demas.begin(); i != demas.end(); i++) {
		    if((*i)->index == num) {
			    return (*i);
			}
		}
	}

	return NULL;
}

Jugador *Juego::getJugadorByNombre(std::string nombre)
{
    if(demas.size() > 0) {
		std::list<Jugador*>::iterator i;
		for(i = demas.begin(); i != demas.end(); i++) {
		    if(!(*i)->nombre.compare(nombre)) {
			    return (*i);
			}
		}
	}

	return NULL;
}

void Juego::addMensaje(Mensaje *msg)
{
	mensajes.push_back(msg);
}

void Juego::quitMensaje(Mensaje *msg)
{
	mensajes.remove(msg);
}

void Juego::MensajesLoop()
{
	if(loopMsgActivo) {
		std::list<Jugador*>::iterator i;
		for(i = demas.begin(); i != demas.end(); i++) {
			(*i)->mensajesMostrados = 0;
		}
		player->mensajesMostrados = 0;

		if(mensajes.size() > 0) {
			std::list<Mensaje*>::reverse_iterator i;
			for(i = mensajes.rbegin(); i != mensajes.rend(); i++) {
				pantalla->DibujarMensaje((*i)->msg, (*i)->jg);
				(*i)->jg->mensajesMostrados++;
			}
		}

		if(mensajes.size() > 0) {
			std::list<Mensaje*>::iterator i;
			for(i = mensajes.begin(); i != mensajes.end(); i++) {
				if((*i)->vida > 150) {
					mensajes.erase(i);
					break;
				} else {
					(*i)->vida++;
				}
			}
		}
	}
}

void Juego::nextAnimationFrame()
{
	player->nextWalkingFrame();

	std::list<Jugador*>::iterator i;
	for(i = demas.begin(); i != demas.end(); i++) {
		(*i)->nextWalkingFrame();
	}
}
