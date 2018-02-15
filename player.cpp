#include "player.h"
#include "game.h"

player::player()
{
	x = 30;
	y = 36;
	clase = 1;
	traje = 1;
	hp = mp = nivel = mensajesMostrados = mensajesActuales = 0;
	index = -1;
	spriteState = 4;
	moviendo = false;
	v_offset = 0;
	h_offset = 0;
    nombre = "Undefined";
}

void player::nextWalkingFrame()
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

void player::setPosition(int nx, int ny)
{
    this->x = nx;
    this->y = ny;
}

void player::moveLeft()
{
	moviendo = true;
	if(h_offset == 0 && v_offset == 0) {
		this->x--;
		spriteState = 6;
		h_offset = 1;
    }
}

void player::moveRight()
{
	moviendo = true;
	if(h_offset == 0 && v_offset == 0) {
		this->x++;
		spriteState = 9;
		h_offset = -1;
	}
}

void player::moveUp()
{
	moviendo = true;
	if(v_offset == 0 && h_offset == 0) {
		this->y--;
		spriteState = 0;
		v_offset = 1;
    }
}

void player::moveDown()
{
	moviendo = true;
	if(v_offset == 0 && h_offset == 0) {
		spriteState = 3;
		this->y++;
		v_offset = -1;
    }
}
