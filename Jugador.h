#ifndef __H_JUGADOR__
#define __H_JUGADOR__

#include <string>

using namespace std;

class Jugador {
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

	Jugador();
	~Jugador() {}

	void nextWalkingFrame();
	void setPosition(int nx, int ny);

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
};

#endif
