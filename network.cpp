#include "network.h"
#include "error.h"
#include "user_interface.h"
#include "player.h"
#include "map.h"
#include "game.h"
#include "graphics.h"

#include <list>
#include <string>
#include <SDL_net.h>

extern user_interface *uiInstance;
extern player *playerInstance;
extern map *mapaActual;
extern game *gameInstance;
extern graphics *pantalla;
extern std::string path;

network::network(void) {
	this->status = 0;
	this->sock = NULL;
	this->intentoConexion = 0;
}

network::~network(void) {
    if(this->status > 0) {
   	    SDLNet_UDP_Close(sock);
   	    sock = NULL;
	    //SDLNet_FreePacket(packenv);
	    SDLNet_FreePacket(packrec);
    }

	this->paraEnviar.clear();
	SDLNet_Quit();
}

int network::connectToTheServer() {
	this->status = 1;

	if(SDLNet_Init() < 0) {
		Error::Log(SDLNet_GetError(), 3);
		this->status = 0;
		return 0;
	}
	if(!(this->packenv = SDLNet_AllocPacket(512))) {
		Error::Log(SDLNet_GetError(), 3);
		this->status = 0;
		return 0;
	}
	if(!(this->packrec = SDLNet_AllocPacket(512))) {
		Error::Log(SDLNet_GetError(), 3);
		this->status = 0;
		return 0;
	}

	if(!(this->sock = SDLNet_UDP_Open(0))) {
		Error::Log(SDLNet_GetError(), 3);
		this->status = 0;
		return 0;
	}

	#ifndef __LOCAL_NETWORK__
	if(SDLNet_ResolveHost(&this->srv, "nailsoficial.no-ip.info", 1411) == -1)
	{
		Error::Log(SDLNet_GetError(), 3);
		this->status = 0;
		return 0;
	}
	#else
	if(SDLNet_ResolveHost(&this->srv, NULL, 1411) == -1)
	{
		Error::Log(SDLNet_GetError(), 3);
		this->status = 0;
		return 0;
	}
	#endif

	intentoConexion = 1;
	return 1;
}

void network::sendLoop() {
    while(gameInstance->abierto) {

        if((int)this->paraEnviar.size() > 0 && this->sock != NULL) {
            std::list<std::string>::iterator it = this->paraEnviar.begin();
		    this->packenv->data = (Uint8*)(*it).c_str();
		    this->packenv->address.host = this->srv.host;
		    this->packenv->address.port = this->srv.port;
		    this->packenv->len = strlen((char *)this->packenv->data) + 1;
		    SDLNet_UDP_Send(this->sock, -1, this->packenv);
		    this->paraEnviar.erase(it);
	    }

	    SDL_Delay(10);
	}
}

void network::recieveLoop() {
    while(gameInstance->abierto) {
        if(status != 0 && this->sock != NULL) {
	        while(SDLNet_UDP_Recv(this->sock, this->packrec)) {
		        this->traducirPaquete((char *)this->packrec->data);
	        }
	    }

	    SDL_Delay(10);
	}
}

void network::sendPacket(std::string pa) {
	this->paraEnviar.push_back(pa);
}

////////////////////////////////////////////
//// TRADUCIR PAQUETES
////////////////////////////////////////////
void network::traducirPaquete(std::string data) {
    std::string buffer = data.substr(4);
    int tipo = (data.at(0)-48)*10 + (data.at(2)-48);

    if(tipo == 0) {
    	t_personajeSeFue(buffer);
    } else if(tipo == 10) {
		t_respuestaLogin(buffer);
	} else if(tipo == 11) {
		t_obtenerlistaPersonajes(buffer);
	} else if(tipo == 12) {
        t_respuestaCrearPersonaje(buffer);
	} else if(tipo == 13) {
		t_respuestaIdentificado(buffer);
	} else if(tipo == 20) {
		t_nuevoJugadorConectado(buffer);
	} else if(tipo == 21) {
		t_jugadorSeMovio(buffer);
	} else if(tipo == 30) {
		t_mensajeEnviado(buffer);
	} else if(tipo == 40) {
		t_cambiarMapa(buffer);
	} else if(tipo == 99) {
		t_expulsadoDelServer(buffer);
	} else {
		Error::Log("Se recibio un paquete sin identificar", 1);
	}
}

void network::t_personajeSeFue(std::string data) {
	gameInstance->removePlayer(gameInstance->getPlayerByNombre(data));
}

void network::t_respuestaLogin(std::string data) {
	intentoConexion = 0;

	if(!data.compare("TRUE")) {
		uiInstance->blocked = false;
		uiInstance->getContainer(1)->getLabel(0)->cambiarTexto("HA INGRESADO CORRECTAMENTE");
		uiInstance->closeContainer(0);
		gameInstance->logged = true;
	    sendPacket("1_1_NOTHING");
	    container *ch = new container;
	    ch->SetCont(260, 250, 500, 300);
	    uiInstance->addContainer(ch);
	} else if(!data.compare("FALSE")) {
		uiInstance->getContainer(1)->getLabel(0)->cambiarTexto("DATOS DE IDENTIFICACION INCORRECTOS");
		uiInstance->blocked = false;
	} else if(!data.compare("NOTVERSION")) {
		uiInstance->getContainer(1)->getLabel(0)->cambiarTexto("VERSION DEL CLIENTE INCORRECTA");
		uiInstance->blocked = false;
	} else { // NOTSERVER
		uiInstance->getContainer(1)->getLabel(0)->cambiarTexto("EL SERVIDOR ESTA DESCONECTADO");
		uiInstance->blocked = false;
	}
}

void network::t_obtenerlistaPersonajes(std::string data) {
	// Variables de datos
	string resto, nombre, clase, traje, hp, mp;
    // Personaje numero...
	int charnum = atoi(data.substr(0, 1).c_str())-1;

    // Datos del personaje
	data = data.substr(2, data.length()+1);

	nombre = data.substr(0, data.find("_"));
	resto = data.substr(data.find("_")+1);
	clase = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	traje = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	hp = resto.substr(0, resto.find("_"));
	mp = resto.substr(resto.find("_")+1);

	int nivel = 30;
	///////////////////////

    container *ch = uiInstance->getContainer(1);

	if(!nombre.compare("NULL")) {
		image *n = new image(path+"data/chars_avatar/nada.png");
        n->set(25 + (charnum * 155), 20);
		ch->Add(n);

		button *ne = new button("Nuevo personaje");
		ne->Set(25+(charnum*155), 265, 137, 20);
		ne->function = &user_interface::e_pantallaNuevoPersonaje;
		ch->Add(ne);
	} else {
		char *buffer = new char[128];
		strcpy(buffer, nombre.c_str());
		label *nm = new label(buffer, 8);
		nm->Set(25+(charnum*155), 205);
		ch->Add(nm);

		label *where = new label("MAPA CENTRAL", 8);
		where->Set(25+(charnum*155), 215);
		ch->Add(where);

		image *n = new image(path+"data/chars_avatar/" + traje + ".png");
        n->set(25 + (charnum * 155), 20);
		ch->Add(n);

		if(!atoi(clase.c_str())) {
			label *claselb = new label("GUERRERO", 8);
			claselb->Set(25+(charnum*155), 225);
			ch->Add(claselb);
		} else {
			label *claselb = new label("MAGO", 8);
			claselb->Set(25+(charnum*155), 225);
			ch->Add(claselb);
		}

		char *lvl = new char[64];
		sprintf(lvl, "Nivel: %d", nivel);
		label *niv = new label(lvl, 8);
		niv->Set(25+(charnum*155), 235);
		ch->Add(niv);

		char *vym = new char[64];
		sprintf(vym, "HP: %s MP: %s", hp.c_str(), mp.c_str());
		label *hm = new label(vym, 8);
		hm->Set(25+(charnum*155), 245);
		ch->Add(hm);

		button *ent = new button("Entrar");
		ent->Set(45+(charnum*155), 265, 100, 20);
		ent->function = &user_interface::e_conectarConPersonaje;
		ent->parametro = charnum;
		ch->Add(ent);
	}

	uiInstance->getContainer(0)->getLabel(0)->cambiarTexto("ELIGE UN PERSONAJE PARA COMENZAR");

	if(charnum == 2) {
	    container *salir = new container(800, 700, 200, 50);
	    button *exit = new button("Salir del juego");
		    exit->Set(25, 15, 150, 20);
		    exit->function = &user_interface::e_cerrarJuego;
	    salir->Add(exit);
	    uiInstance->addContainer(salir);
	}
}

void network::t_respuestaCrearPersonaje(std::string data) {
	if(!data.compare("TRUE")) {
		uiInstance->getContainer(3)->getLabel(0)->cambiarTexto("PERSONAJE CREADO CORRECTAMENTE");
		uiInstance->closeContainer(3);
		uiInstance->closeContainer(2);
		uiInstance->closeContainer(1);

		container *ch = new container;
	    ch->SetCont(260, 250, 500, 300);
	    uiInstance->addContainer(ch);
	} else {
		uiInstance->getContainer(3)->getLabel(0)->cambiarTexto("ERROR AL CREAR EL PERSONAJE");
		uiInstance->closeContainer(3);
	}
}

void network::t_respuestaIdentificado(std::string data) {
	string resto, xpos, ypos, clase, traje, mapa, nombre;
	xpos = data.substr(0, data.find("_"));
	resto = data.substr(data.find("_")+1);
	ypos = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	clase = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	traje = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	mapa = resto.substr(0, resto.find("_"));
	nombre = resto.substr(resto.find("_")+1);

	playerInstance->nombre = nombre;
	playerInstance->clase = atoi(clase.c_str());
	playerInstance->traje = atoi(traje.c_str());
	playerInstance->setPosition(atoi(xpos.c_str()), atoi(ypos.c_str()));
	uiInstance->closeContainer(0);

	gameInstance->cambiarMapa(mapa);
	gameInstance->playing = true;
	uiInstance->writing = false;
	// SDL_EnableKeyRepeat(100, 30);
}

void network::t_nuevoJugadorConectado(std::string data) {
	string resto, xpos, ypos, clase, traje, nombre;
	xpos = data.substr(0, data.find("_"));
	resto = data.substr(data.find("_")+1);
	ypos = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	clase = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	traje = resto.substr(0, resto.find("_"));
	nombre = resto.substr(resto.find("_")+1);

	player *charac = new player;
	charac->setPosition(atoi(xpos.c_str()), atoi(ypos.c_str()));
	charac->nombre = nombre;
	charac->traje = atoi(traje.c_str());
	charac->clase = atoi(clase.c_str());
	gameInstance->newPlayer(charac);
}

void network::t_jugadorSeMovio(std::string data) {
	size_t found = data.find("_");
	size_t foundtwo = data.rfind("_");
	int newx, newy;
	std::string name = data.substr(foundtwo+1);

	player *jd = gameInstance->getPlayerByNombre(name);
	if(jd != NULL) {
		newx = atoi(data.substr(0, found).c_str());
		newy = atoi(data.substr(found+1, foundtwo).c_str());

		if(newx == jd->x+1) { jd->h_offset = -1; jd->x++; jd->spriteState = 9;
		} else if(newx == jd->x-1) { jd->h_offset = 1; jd->x--; jd->spriteState = 6;
		} else if(newy == jd->y+1) { jd->v_offset = -1; jd->y++; jd->spriteState = 3;
		} else if(newy == jd->y-1) { jd->v_offset = 1; jd->y--; jd->spriteState = 0; }
	}
}

void network::t_mensajeEnviado(std::string data) {
	Mensaje *tosend = new Mensaje;

	size_t found = data.rfind("_");
	std::string msg = data.substr(0, found);
	std::string name = data.substr(found+1);
	if(!name.compare(playerInstance->nombre)) {
		tosend->jg = playerInstance;
	} else {
		tosend->jg = gameInstance->getPlayerByNombre(name);
	}

	char *buffer = new char[256];
	strcpy(buffer, msg.c_str());
	tosend->msg = buffer;
	tosend->vida = 0;
	gameInstance->addMensaje(tosend);

	tosend->jg->mensajesActuales++;
}

void network::t_cambiarMapa(std::string data) {
	string resto, mapa, xpos,  ypos;
	mapa = data.substr(0, data.find("_"));
	resto = data.substr(data.find("_")+1);
	xpos = resto.substr(0, resto.find("_"));
	ypos = resto.substr(resto.find("_")+1);

	for(int x = 0; x < gameInstance->jugadores(); x++) {
		gameInstance->removePlayer(gameInstance->getPlayerByIndex(x));

	}
	gameInstance->cambiarMapa(mapa);
	playerInstance->setPosition(atoi(xpos.c_str()), atoi(ypos.c_str()));
}

void network::t_expulsadoDelServer(std::string data) {
	gameInstance->playing = false;
	gameInstance->logged = false;

	container *cn = new container(260, 320, 500, 50);
	label *a = new label("HA SIDO EXPULSADO DEL SERVIDOR", 16);
		a->Set(19, 19);
		cn->Add(a);
	uiInstance->addContainer(cn);
}
