#include "Jugador.h"
#include "Network.h"
#include "Juego.h"

#include <string>
#include <sstream>

extern Network *net;
extern Juego *game;

Jugador::Jugador()
{
	x = y = hp = mp = nivel = traje = clase = mensajesMostrados = mensajesActuales = 0;
	index = -1;
	spriteState = 4;
	moviendo = false;
	v_offset = 0;
	h_offset = 0;
}

void Jugador::nextWalkingFrame()
{
	if(h_offset == 17 || v_offset == 17 || h_offset == -17 || v_offset == -17) {
		switch(spriteState) {
			case 0: spriteState = 2; break;
			case 2: spriteState = 0; break;
			case 3: spriteState = 5; break;
			case 5: spriteState = 3; break;
			case 6: spriteState = 8; break;
			case 8: spriteState = 6; break;
			case 9: spriteState = 11; break;
			case 11: spriteState = 9; break;
			default: break;
		}
	}

	if(h_offset > 0) h_offset += 4;
	if(h_offset < 0) h_offset -= 4;
	if(v_offset > 0) v_offset += 4;
	if(v_offset < 0) v_offset -= 4;

	if(h_offset+4 > 32) {
		h_offset = 0;
		if(!moviendo) spriteState = 7;
	} else if(h_offset+4 < -32) {
		h_offset = 0;
		if(!moviendo) spriteState = 10;
	} else if(v_offset+4 > 32) {
		v_offset = 0;
		if(!moviendo) spriteState = 1;
	} else if(v_offset+4 < -32) {
		v_offset = 0;
		if(!moviendo) spriteState = 4;
	}
}

void Jugador::setPosition(int nx, int ny)
{
    this->x = nx;
    this->y = ny;
}

void Jugador::moveLeft()
{
	moviendo = true;
	if(game->mapaActual->comprobarTilePisable(x-1, y) && h_offset == 0 && v_offset == 0) {
		this->x--;
		spriteState = 6;
		h_offset = 1;
		std::string buffer = "2_0_";
		std::stringstream aux;
		aux << this->x;
		buffer.append(aux.str());
		buffer.append("_");
		aux.str("");
		aux << this->y;
		buffer.append(aux.str());
		net->sendPacket(buffer);
    }
}

void Jugador::moveRight()
{
	moviendo = true;
	if(game->mapaActual->comprobarTilePisable(x+1, y) && h_offset == 0 && v_offset == 0) {
		this->x++;
		spriteState = 9;
		h_offset = -1;
		std::string buffer = "2_0_";
		std::stringstream aux;
		aux << this->x;
		buffer.append(aux.str());
		buffer.append("_");
		aux.str("");
		aux << this->y;
		buffer.append(aux.str());
		net->sendPacket(buffer);
	}
}

void Jugador::moveUp()
{
	moviendo = true;
	if(game->mapaActual->comprobarTilePisable(x, y-1) && v_offset == 0 && h_offset == 0) {
		this->y--;
		spriteState = 0;
		v_offset = 1;
		std::string buffer = "2_0_";
		std::stringstream aux;
		aux << this->x;
		buffer.append(aux.str());
		buffer.append("_");
		aux.str("");
		aux << this->y;
		buffer.append(aux.str());
		net->sendPacket(buffer);
    }
}

void Jugador::moveDown()
{
	moviendo = true;
	if(game->mapaActual->comprobarTilePisable(x, y+1) && v_offset == 0 && h_offset == 0) {
		spriteState = 3;
		this->y++;
		v_offset = -1;
		std::string buffer = "2_0_";
		std::stringstream aux;
		aux << this->x;
		buffer.append(aux.str());
		buffer.append("_");
		aux.str("");
		aux << this->y;
		buffer.append(aux.str());
		net->sendPacket(buffer);
    }
}
