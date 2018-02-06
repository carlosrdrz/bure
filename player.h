#pragma once

#include <string>

using namespace std;

class player {
  public:
  	// Caracteristicas del personaje
	int x, y, hp, mp, nivel, traje, clase;
	// Numero de personaje
	int index;
	// Variables de mensajes
	int mensajesActuales, mensajesMostrados;
	// Variable de animacion de movimiento
	int spriteState;
	bool moviendo;
	// Variables de movimiento en mapa
	int v_offset, h_offset;
	// Nombre del personaje
	string nombre;

	player();
	~player() {}

	void nextWalkingFrame();
	void setPosition(int nx, int ny);

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
};
