#ifndef __H_GRAFICOS__
#define __H_GRAFICOS__

#include "SDL.h"
#include "SDL_ttf.h"
#include "Mapa.h"
#include "Interface.h"
#include "Jugador.h"

#include <list>

using namespace std;

class Graficos {
public:
	Graficos(void);
	~Graficos(void);

	void Dibujar(int tile, int x, int y, int h, int v, bool entera, bool capa);
	void Dibujar(Mapa *m, bool capa);
	void Dibujar(Interface *i);
	void DibujarJugadores();
	void DibujarMensaje(char *msg, Jugador *jd);

	void RenderBg();

	void AbrirFuente(int tamano);
	void Limpiar(void);
	void Volcar(void);

private:
	SDL_Surface *pantalla, *bbuffer, *tiles, *personajes, *elementos, *gui_images, *bg;
	SDL_Color bgcolor, fuentec;
	TTF_Font *fuente;
	int tam_fuente;
};

#endif
