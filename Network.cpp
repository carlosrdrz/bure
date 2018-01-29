#include "Network.h"
#include "Error.h"
#include "Interface.h"
#include "Jugador.h"
#include "Mapa.h"
#include "Juego.h"
#include "Graficos.h"

#include <list>
#include <string>
#include <SDL_net.h>

extern Interface *interfaz;
extern Jugador *player;
extern Mapa *mapaActual;
extern Juego *game;
extern Graficos *pantalla;
extern std::string path;

Network::Network(void) {
	this->status = 0;
	this->sock = NULL;
	this->intentoConexion = 0;
}

Network::~Network(void) {
    if(this->status > 0) {
   	    SDLNet_UDP_Close(sock);
   	    sock = NULL;
	    //SDLNet_FreePacket(packenv);
	    SDLNet_FreePacket(packrec);
    }

	this->paraEnviar.clear();
	SDLNet_Quit();
}

int Network::connectToTheServer() {
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

void Network::sendLoop() {
    while(game->abierto) {

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

void Network::recieveLoop() {
    while(game->abierto) {
        if(status != 0 && this->sock != NULL) {
	        while(SDLNet_UDP_Recv(this->sock, this->packrec)) {
		        this->traducirPaquete((char *)this->packrec->data);
	        }
	    }

	    SDL_Delay(10);
	}
}

void Network::sendPacket(std::string pa) {
	this->paraEnviar.push_back(pa);
}

////////////////////////////////////////////
//// TRADUCIR PAQUETES
////////////////////////////////////////////
void Network::traducirPaquete(std::string data) {
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

void Network::t_personajeSeFue(std::string data) {
	game->quitarJugador(game->getJugadorByNombre(data));
}

void Network::t_respuestaLogin(std::string data) {
	intentoConexion = 0;

	if(!data.compare("TRUE")) {
		interfaz->bloqueado = false;
		interfaz->getContainer(1)->getLabel(0)->cambiarTexto("HA INGRESADO CORRECTAMENTE");
		interfaz->closeContainer(0);
		game->logged = true;
	    sendPacket("1_1_NOTHING");
	    Container *ch = new Container;
	    ch->SetCont(260, 250, 500, 300);
	    interfaz->addContainer(ch);
	} else if(!data.compare("FALSE")) {
		interfaz->getContainer(1)->getLabel(0)->cambiarTexto("DATOS DE IDENTIFICACION INCORRECTOS");
		interfaz->bloqueado = false;
	} else if(!data.compare("NOTVERSION")) {
		interfaz->getContainer(1)->getLabel(0)->cambiarTexto("VERSION DEL CLIENTE INCORRECTA");
		interfaz->bloqueado = false;
	} else { // NOTSERVER
		interfaz->getContainer(1)->getLabel(0)->cambiarTexto("EL SERVIDOR ESTA DESCONECTADO");
		interfaz->bloqueado = false;
	}
}

void Network::t_obtenerlistaPersonajes(std::string data) {
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

    Container *ch = interfaz->getContainer(1);

	if(!nombre.compare("NULL")) {
		Imagen *n = new Imagen(path+"data/chars_avatar/nada.png");
		n->Set(25+(charnum*155), 20);
		ch->Add(n);

		Boton *ne = new Boton("Nuevo personaje");
		ne->Set(25+(charnum*155), 265, 137, 20);
		ne->function = &Interface::e_pantallaNuevoPersonaje;
		ch->Add(ne);
	} else {
		char *buffer = new char[128];
		strcpy(buffer, nombre.c_str());
		Label *nm = new Label(buffer, 8);
		nm->Set(25+(charnum*155), 205);
		ch->Add(nm);

		Label *where = new Label("MAPA CENTRAL", 8);
		where->Set(25+(charnum*155), 215);
		ch->Add(where);

		Imagen *n = new Imagen(path+"data/chars_avatar/" + traje + ".png");
		n->Set(25+(charnum*155), 20);
		ch->Add(n);

		if(!atoi(clase.c_str())) {
			Label *claselb = new Label("GUERRERO", 8);
			claselb->Set(25+(charnum*155), 225);
			ch->Add(claselb);
		} else {
			Label *claselb = new Label("MAGO", 8);
			claselb->Set(25+(charnum*155), 225);
			ch->Add(claselb);
		}

		char *lvl = new char[64];
		sprintf(lvl, "Nivel: %d", nivel);
		Label *niv = new Label(lvl, 8);
		niv->Set(25+(charnum*155), 235);
		ch->Add(niv);

		char *vym = new char[64];
		sprintf(vym, "HP: %s MP: %s", hp.c_str(), mp.c_str());
		Label *hm = new Label(vym, 8);
		hm->Set(25+(charnum*155), 245);
		ch->Add(hm);

		Boton *ent = new Boton("Entrar");
		ent->Set(45+(charnum*155), 265, 100, 20);
		ent->function = &Interface::e_conectarConPersonaje;
		ent->parametro = charnum;
		ch->Add(ent);
	}

	interfaz->getContainer(0)->getLabel(0)->cambiarTexto("ELIGE UN PERSONAJE PARA COMENZAR");

	if(charnum == 2) {
	    Container *salir = new Container(800, 700, 200, 50);
	    Boton *exit = new Boton("Salir del juego");
		    exit->Set(25, 15, 150, 20);
		    exit->function = &Interface::e_cerrarJuego;
	    salir->Add(exit);
	    interfaz->addContainer(salir);
	}
}

void Network::t_respuestaCrearPersonaje(std::string data) {
	if(!data.compare("TRUE")) {
		interfaz->getContainer(3)->getLabel(0)->cambiarTexto("PERSONAJE CREADO CORRECTAMENTE");
		interfaz->closeContainer(3);
		interfaz->closeContainer(2);
		interfaz->closeContainer(1);

		Container *ch = new Container;
	    ch->SetCont(260, 250, 500, 300);
	    interfaz->addContainer(ch);
	} else {
		interfaz->getContainer(3)->getLabel(0)->cambiarTexto("ERROR AL CREAR EL PERSONAJE");
		interfaz->closeContainer(3);
	}
}

void Network::t_respuestaIdentificado(std::string data) {
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

	player->nombre = nombre;
	player->clase = atoi(clase.c_str());
	player->traje = atoi(traje.c_str());
	player->setPosition(atoi(xpos.c_str()), atoi(ypos.c_str()));
	interfaz->closeContainer(0);

	game->cambiarMapa(mapa);
	game->jugando = true;
	interfaz->writing = false;
	SDL_EnableKeyRepeat(100, 30);
}

void Network::t_nuevoJugadorConectado(std::string data) {
	string resto, xpos, ypos, clase, traje, nombre;
	xpos = data.substr(0, data.find("_"));
	resto = data.substr(data.find("_")+1);
	ypos = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	clase = resto.substr(0, resto.find("_"));
	resto = resto.substr(resto.find("_")+1);
	traje = resto.substr(0, resto.find("_"));
	nombre = resto.substr(resto.find("_")+1);

	Jugador *charac = new Jugador;
	charac->setPosition(atoi(xpos.c_str()), atoi(ypos.c_str()));
	charac->nombre = nombre;
	charac->traje = atoi(traje.c_str());
	charac->clase = atoi(clase.c_str());
	game->nuevoJugador(charac);
}

void Network::t_jugadorSeMovio(std::string data) {
	size_t found = data.find("_");
	size_t foundtwo = data.rfind("_");
	int newx, newy;
	std::string name = data.substr(foundtwo+1);

	Jugador *jd = game->getJugadorByNombre(name);
	if(jd != NULL) {
		newx = atoi(data.substr(0, found).c_str());
		newy = atoi(data.substr(found+1, foundtwo).c_str());

		if(newx == jd->x+1) { jd->h_offset = -1; jd->x++; jd->spriteState = 9;
		} else if(newx == jd->x-1) { jd->h_offset = 1; jd->x--; jd->spriteState = 6;
		} else if(newy == jd->y+1) { jd->v_offset = -1; jd->y++; jd->spriteState = 3;
		} else if(newy == jd->y-1) { jd->v_offset = 1; jd->y--; jd->spriteState = 0; }
	}
}

void Network::t_mensajeEnviado(std::string data) {
	Mensaje *tosend = new Mensaje;

	size_t found = data.rfind("_");
	std::string msg = data.substr(0, found);
	std::string name = data.substr(found+1);
	if(!name.compare(player->nombre)) {
		tosend->jg = player;
	} else {
		tosend->jg = game->getJugadorByNombre(name);
	}

	char *buffer = new char[256];
	strcpy(buffer, msg.c_str());
	tosend->msg = buffer;
	tosend->vida = 0;
	game->addMensaje(tosend);

	tosend->jg->mensajesActuales++;
}

void Network::t_cambiarMapa(std::string data) {
	string resto, mapa, xpos,  ypos;
	mapa = data.substr(0, data.find("_"));
	resto = data.substr(data.find("_")+1);
	xpos = resto.substr(0, resto.find("_"));
	ypos = resto.substr(resto.find("_")+1);

	for(int x = 0; x < game->jugadores(); x++) {
	    	game->quitarJugador(game->getJugadorByIndex(x));

	}
	game->cambiarMapa(mapa);
	player->setPosition(atoi(xpos.c_str()), atoi(ypos.c_str()));
}

void Network::t_expulsadoDelServer(std::string data) {
	game->jugando = false;
	game->logged = false;

	Container *cn = new Container(260, 320, 500, 50);
	Label *a = new Label("HA SIDO EXPULSADO DEL SERVIDOR", 16);
		a->Set(19, 19);
		cn->Add(a);
	interfaz->addContainer(cn);
}
