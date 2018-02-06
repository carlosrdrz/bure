#include "game.h"
#include "map.h"
#include "graphics.h"
#include "player.h"
#include "error.h"

#include <string>

extern graphics *pantalla;
extern player *playerInstance;

game::game()
{
    currentMap = nullptr;
    abierto = true;
    playing = false;
    loopMsgActivo = true;
    logged = false;
    princp = true;
    princp_parado = false;
}

game::~game()
{
	demas.clear();
	mensajes.clear();
}

void game::cerrar()
{
    abierto = false;
    playing = false;
}

void game::cambiarMapa(std::string archivo)
{
	// Paramos thread principal de dibujo
	// princp = false;
	// while(!princp_parado);
	delete currentMap;
	currentMap = new map(archivo);
	princp = true;
}

void game::newPlayer(player *pl)
{
    pl->index = (int)demas.size();
    demas.push_back(pl);
}

void game::removePlayer(player *pl)
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

int game::jugadores()
{
    return (int)demas.size();
}

player *game::getPlayerByIndex(int num)
{
    if(demas.size() > 0) {
		std::list<player*>::iterator i;
		for(i = demas.begin(); i != demas.end(); i++) {
		    if((*i)->index == num) {
			    return (*i);
			}
		}
	}

	return NULL;
}

player *game::getPlayerByNombre(std::string nombre)
{
    if(demas.size() > 0) {
		std::list<player*>::iterator i;
		for(i = demas.begin(); i != demas.end(); i++) {
		    if(!(*i)->nombre.compare(nombre)) {
			    return (*i);
			}
		}
	}

	return NULL;
}

void game::addMensaje(Mensaje *msg)
{
	mensajes.push_back(msg);
}

void game::quitMensaje(Mensaje *msg)
{
	mensajes.remove(msg);
}

void game::MensajesLoop()
{
	if(loopMsgActivo) {
		std::list<player*>::iterator i;
		for(i = demas.begin(); i != demas.end(); i++) {
			(*i)->mensajesMostrados = 0;
		}
		playerInstance->mensajesMostrados = 0;

		if(mensajes.size() > 0) {
			std::list<Mensaje*>::reverse_iterator i;
			for(i = mensajes.rbegin(); i != mensajes.rend(); i++) {
                pantalla->drawMessage((*i)->msg, (*i)->jg);
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

void game::nextAnimationFrame()
{
	playerInstance->nextWalkingFrame();

	std::list<player*>::iterator i;
	for(i = demas.begin(); i != demas.end(); i++) {
		(*i)->nextWalkingFrame();
	}
}
