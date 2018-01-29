#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Mapa.h"
#include "Error.h"
#include "Interface.h"
#include "Jugador.h"
#include "Juego.h"
#include "Graficos.h"
#include "Config.h"

#include <list>

#define T_TAMANO 32

extern Interface *interfaz;
extern Juego *game;
extern Jugador *player;
extern Config *cfg;
extern std::string path;

Graficos::Graficos()
{
	// Iniciamos el video, a 1024x768, en ventana
	if(cfg->getBoolValueOf("fullscreen")) {
		pantalla = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	} else {
		pantalla = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	}
	if(pantalla == NULL) {
		Error::Log(SDL_GetError(), 3);
	}

	// Crear el buffer secundario
	Uint8 rmask = 255, gmask = 255, bmask = 255, amask = 255;
	SDL_Surface *bbufferd = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, 1024, 768, 32, rmask, gmask, bmask, amask);
	if(bbufferd == NULL) {
		Error::Log(SDL_GetError(), 3);
	}
	bbuffer = SDL_DisplayFormat(bbufferd);
	SDL_FreeSurface(bbufferd);

	// Cargamos el archivo de las tiles en su surface
	tiles = IMG_Load((path+"data/tiles.png").c_str());
	if(tiles == NULL) {
		Error::Log(SDL_GetError(), 2);
	}

	// El archivo de sprites de personajes
	personajes = IMG_Load((path+"data/personajes.png").c_str());
	if(personajes == NULL) {
		Error::Log(SDL_GetError(), 2);
	}

	// Archivo de tiles de elementos
	elementos = IMG_Load((path+"data/elements.png").c_str());
	if(elementos == NULL) {
		Error::Log(SDL_GetError(), 2);
	}

	// El archivo del gui
	gui_images = IMG_Load((path+"data/gui.png").c_str());
	if(gui_images == NULL) {
		Error::Log(SDL_GetError(), 2);
	}

	// Iniciamos SDL_ttf y cargamos la fuente
	if(TTF_Init() < 0) {
		Error::Log(SDL_GetError(), 2);
	}
	fuente = TTF_OpenFont((path+"data/pixel_font.ttf").c_str(), 16);
	if(!fuente) {
    	Error::Log(TTF_GetError(), 2);
	}

	this->tam_fuente = 16;

	fuentec.r = 255;
	fuentec.g = 255;
	fuentec.b = 255;
	bgcolor.r = 114;
	bgcolor.g = 107;
	bgcolor.b = 82;

	// Cargar fondo
	bg = IMG_Load((path+"data/background.jpg").c_str());
}

Graficos::~Graficos(void)
{
	SDL_FreeSurface(pantalla);
	SDL_FreeSurface(bbuffer);
	SDL_FreeSurface(tiles);
	SDL_FreeSurface(gui_images);
	SDL_FreeSurface(personajes);
	SDL_FreeSurface(elementos);
	SDL_FreeSurface(bg);

	TTF_CloseFont(fuente);
	TTF_Quit();
}

void Graficos::Limpiar()
{
	if(SDL_FillRect(bbuffer, NULL, SDL_MapRGB(bbuffer->format, 0, 0, 0)) < 0) {
		Error::Log((char *)"Error al limpiar la pantalla", 1);
	}
}

void Graficos::Dibujar(int tile, int x, int y, int h, int v, bool entera, bool capa = false) {
	SDL_Rect origen, destino;

	int tilex = 0;
	int tiley = 0;
	if(!capa) {
		while(tile > 7) {
			tile -= 7;
			tiley += 32;
		}
		tilex = (tile-1)*32;
	} else {
		while(tile > 8) {
			tile -= 8;
			tiley += 32;
		}
		tilex = (tile-1)*32;
	}

	if(entera) {
		origen.x = tilex;
		origen.y = tiley;
		origen.w = T_TAMANO;
		origen.h = T_TAMANO;

		destino.x = x+h;
		destino.y = y+v;
		destino.w = T_TAMANO;
		destino.h = T_TAMANO;
	} else {
		if(h > 0) {
			origen.x = tilex+32-h;
			origen.y = tiley;
			origen.w = T_TAMANO-(32-h);
			origen.h = T_TAMANO;

			destino.x = x;
			destino.y = y;
			destino.w = T_TAMANO-(32-h);
			destino.h = T_TAMANO;
		} else if(h < 0) {
			origen.x = tilex;
			origen.y = tiley;
			origen.w = h;
			origen.h = T_TAMANO;

			destino.x = x;
			destino.y = y;
			destino.w = h;
			destino.h = T_TAMANO;
		} else if(v > 0) {
			origen.x = tilex;
			origen.y = tiley+(32-v);
			origen.w = T_TAMANO;
			origen.h = v;

			destino.x = x;
			destino.y = y;
			destino.w = T_TAMANO;
			destino.h = v;
		} else if(v < 0) {
			origen.x = tilex;
			origen.y = tiley;
			origen.w = T_TAMANO;
			origen.h = 32-v;

			destino.x = x;
			destino.y = y;
			destino.w = T_TAMANO;
			destino.h = v;
		}
	}

	if(!capa) SDL_BlitSurface(tiles, &origen, bbuffer, &destino);
	else SDL_BlitSurface(elementos, &origen, bbuffer, &destino);
}

void Graficos::Dibujar(Mapa *m, bool capa) {
	int empezar_x, empezar_y = 0;
	int jug_y = player->y;
	int jug_x = player->x;
	int x, y;

	player->moviendo = false;

	if(player->h_offset > 0) {
		empezar_y = 0;
		for(y=(jug_y-12);y<(jug_y+12);y++) {
			if(y >= 0 && y < m->mapa_alto) {
				if(!capa) Dibujar(m->mapa[(jug_x-16)+y*m->mapa_ancho], 0, empezar_y, player->h_offset, player->v_offset, false);
				else Dibujar(m->elementos[(jug_x-16)+y*m->mapa_ancho], 0, empezar_y, player->h_offset, player->v_offset, false, true);
			}
			empezar_y += T_TAMANO;
		}
		jug_x++;
	}

	if(player->h_offset < 0) {
		empezar_y = 0;
		for(y=(jug_y-12);y<(jug_y+12);y++) {
			if(y >= 0 && y < m->mapa_alto) {
				if(!capa) Dibujar(m->mapa[(jug_x+15)+y*m->mapa_ancho], 1024+player->h_offset, empezar_y, player->h_offset, player->v_offset, false);
				else Dibujar(m->elementos[(jug_x+15)+y*m->mapa_ancho], 1024+player->h_offset, empezar_y, player->h_offset, player->v_offset, false, true);
			}
			empezar_y += T_TAMANO;
		}
		jug_x--;
	}
	if(player->v_offset > 0) {
		empezar_x = 0;
		for(x=(jug_x-16);x<(jug_x+16);x++) {
			if(x >= 0 && x < m->mapa_ancho) {
				if(!capa) Dibujar(m->mapa[(x+((jug_y-12)*m->mapa_ancho))], empezar_x, 0, player->h_offset, player->v_offset, false);
				else Dibujar(m->elementos[(x+((jug_y-12)*m->mapa_ancho))], empezar_x, 0, player->h_offset, player->v_offset, false, true);
			}
			empezar_x += T_TAMANO;
		}
		jug_y++;
	}
	if(player->v_offset < 0) {
		empezar_x = 0;
		for(x=(jug_x-16);x<(jug_x+16);x++) {
			if(x >= 0 && x < m->mapa_ancho) {
				if(!capa) Dibujar(m->mapa[(x+((jug_y+11)*m->mapa_ancho))], empezar_x, 768+player->v_offset, player->h_offset, player->v_offset, false);
				else Dibujar(m->elementos[(x+((jug_y+11)*m->mapa_ancho))], empezar_x, 768+player->v_offset, player->h_offset, player->v_offset, false, true);
			}
			empezar_x += T_TAMANO;
		}
		jug_y--;
	}

	empezar_x = empezar_y = 0;

	for(y=(jug_y-12);y<(jug_y+12);y++) {
		for(x=(jug_x-16);x<(jug_x+16);x++) {
			if(x >= 0 && y >= 0 && x < m->mapa_ancho && y < m->mapa_alto) {
				if(!capa) Dibujar(m->mapa[(x+(y*m->mapa_ancho))], empezar_x, empezar_y, player->h_offset, player->v_offset, true);
				else Dibujar(m->elementos[(x+(y*m->mapa_ancho))], empezar_x, empezar_y, player->h_offset, player->v_offset, true, true);
			}
			empezar_x += T_TAMANO;
		}
		empezar_x = 0;
		empezar_y += T_TAMANO;
	}
}

void Graficos::RenderBg() {
	SDL_BlitSurface(bg, NULL, bbuffer, NULL);
}

void Graficos::Dibujar(Interface *i) {
	SDL_Rect destino;
	SDL_Rect origen;

	int contenedor, lb, ib, bon, img;
	for(contenedor = 0; i->getContainer(contenedor) != NULL; contenedor++) {
		if(!i->getContainer(contenedor)->invisible) {
		// Globales para todo el contenedor
		origen.w = 1;
		origen.h = 26;
		origen.y = 0;

		// Fondo del contenedor
		destino.x = i->getContainer(contenedor)->x;
		destino.y = i->getContainer(contenedor)->y;
		destino.w = i->getContainer(contenedor)->ancho;
		destino.h = i->getContainer(contenedor)->alto;
		SDL_FillRect(bbuffer, &destino, SDL_MapRGB(bbuffer->format, bgcolor.r, bgcolor.g, bgcolor.b));

		// Barras horizontales
		destino.w = 32;
		destino.h = 32;

		origen.x = 104;
		destino.y -= 12;
		destino.x--;
		for(int y = 0; y < i->getContainer(contenedor)->ancho; y++) {
		destino.x++;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
		}

		destino.y += i->getContainer(contenedor)->alto;
		destino.x = i->getContainer(contenedor)->x-1;
		for(int y = 0; y < i->getContainer(contenedor)->ancho; y++) {
		destino.x++;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
		}

		// Barras verticales
		origen.x = 130;
		origen.w = 26;
		origen.h = 1;

		destino.x = i->getContainer(contenedor)->x-11;
		destino.y = i->getContainer(contenedor)->y;
		for(int y = 0; y < i->getContainer(contenedor)->alto; y++) {
		destino.y++;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
		}

		destino.x += i->getContainer(contenedor)->ancho-4;
		destino.y = i->getContainer(contenedor)->y;
		for(int y = 0; y < i->getContainer(contenedor)->alto; y++) {
		destino.y++;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
		}

		// Todas las esquinas
		origen.w = 26;
		origen.h = 26;
		destino.w = 26;
		destino.h = 26;

		origen.x = 0;
		origen.y = 0;
		destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->ancho-15;
		destino.y = i->getContainer(contenedor)->y-12;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

		origen.x += 26;
		destino.x -= i->getContainer(contenedor)->ancho-4;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

		origen.x += 26;
		destino.y += i->getContainer(contenedor)->alto;
		destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->ancho-15;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

		origen.x += 26;
		destino.x -= i->getContainer(contenedor)->ancho-4;
		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
		}

		// Labels
		for(lb = 0; i->getContainer(contenedor)->getLabel(lb) != NULL; lb++) {
            if(i->getContainer(contenedor)->getLabel(lb)->getTamano() != this->tam_fuente) {
			    this->AbrirFuente(i->getContainer(contenedor)->getLabel(lb)->getTamano());
            }

			SDL_Surface *texto = TTF_RenderText_Solid(fuente, i->getContainer(contenedor)->getLabel(lb)->getText(), fuentec);

			if(texto != NULL) {
			    destino.x = i->getContainer(contenedor)->getLabel(lb)->x+i->getContainer(contenedor)->x;
			    destino.y = i->getContainer(contenedor)->getLabel(lb)->y+i->getContainer(contenedor)->y;
			    destino.w = texto->w;
			    destino.h = texto->h;

			    SDL_BlitSurface(texto, NULL, bbuffer, &destino);
			    SDL_FreeSurface(texto);
			}
		}

		// InputBoxes
		for(ib = 0; i->getContainer(contenedor)->getIB(ib) != NULL; ++ib) {
			// Se dibuja el input box
			origen.x = (i->getContainer(contenedor)->getIB(ib)->getFocus() && i->getContainer(contenedor)->getFocus() && !i->bloqueado) ? 1 : 3;
			origen.y = 26;
			origen.w = 1;
			origen.h = 14;

			destino.x = i->getContainer(contenedor)->getIB(ib)->x+i->getContainer(contenedor)->x;
			destino.y = i->getContainer(contenedor)->getIB(ib)->y+i->getContainer(contenedor)->y;
			destino.w = 1;
			destino.h = 14;

			SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
			destino.x++;
			origen.x--;
			for(int an = 0; an < i->getContainer(contenedor)->getIB(ib)->ancho-2; an++) {
				SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
				destino.x++;
			}
			origen.x++;
			SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

			// La etiqueta del inputbox
			if(8 != this->tam_fuente) {
				this->AbrirFuente(8);
			}

			SDL_Surface *title = TTF_RenderText_Solid(fuente, i->getContainer(contenedor)->getIB(ib)->getTitulo(), fuentec);
			destino.x = i->getContainer(contenedor)->getIB(ib)->x+i->getContainer(contenedor)->x-title->w-5;
			destino.y += 3;
			SDL_BlitSurface(title, NULL, bbuffer, &destino);

			if(i->getContainer(contenedor)->getIB(ib)->getCaracteres() != 0) {
				SDL_Surface *text;
				if(i->getContainer(contenedor)->getIB(ib)->secreto == 1) {
					char sec[13];
					for(int sd = 0; sd < i->getContainer(contenedor)->getIB(ib)->getCaracteres(); sd++) {
						sec[sd] = 42;
					}
					sec[i->getContainer(contenedor)->getIB(ib)->getCaracteres()] = 0;
					text = TTF_RenderText_Solid(fuente, sec, fuentec);
				} else {
					text = TTF_RenderText_Solid(fuente, i->getContainer(contenedor)->getIB(ib)->getTexto(), fuentec);
				}
				destino.x += title->w+10;
				SDL_BlitSurface(text, NULL, bbuffer, &destino);
				SDL_FreeSurface(text);
			}
			SDL_FreeSurface(title);
		}

		// Botones
		for(bon = 0; i->getContainer(contenedor)->getBoton(bon) != NULL; bon++) {
			Uint32 color;
			if(i->getContainer(contenedor)->getBoton(bon)->press == 0) color = SDL_MapRGB(bbuffer->format, 63, 63, 63);
			else color = SDL_MapRGB(bbuffer->format, 0, 0, 0);

			destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getBoton(bon)->x;
			destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getBoton(bon)->y;
			destino.w = i->getContainer(contenedor)->getBoton(bon)->ancho;
			destino.h = i->getContainer(contenedor)->getBoton(bon)->alto;
			SDL_FillRect(bbuffer, &destino, color);

			if(i->getContainer(contenedor)->getBoton(bon)->press == 0) color = SDL_MapRGB(bbuffer->format, 0, 0, 0);
			else color = SDL_MapRGB(bbuffer->format, 63, 63, 63);

			destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getBoton(bon)->x;
			destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getBoton(bon)->y;
			destino.w = i->getContainer(contenedor)->getBoton(bon)->ancho;
			destino.h = 1;
			SDL_FillRect(bbuffer, &destino, color);

			destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getBoton(bon)->x;
			destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getBoton(bon)->y+i->getContainer(contenedor)->getBoton(bon)->alto;
			destino.w = i->getContainer(contenedor)->getBoton(bon)->ancho;
			destino.h = 1;
			SDL_FillRect(bbuffer, &destino, color);

			destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getBoton(bon)->x;
			destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getBoton(bon)->y;
			destino.w = 1;
			destino.h = i->getContainer(contenedor)->getBoton(bon)->alto;
			SDL_FillRect(bbuffer, &destino, color);

			destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getBoton(bon)->x+i->getContainer(contenedor)->getBoton(bon)->ancho;
			destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getBoton(bon)->y;
			destino.w = 1;
			destino.h = i->getContainer(contenedor)->getBoton(bon)->alto;
			SDL_FillRect(bbuffer, &destino, color);

			// Dibujar la etiqueta
			if(this->tam_fuente != 8) this->AbrirFuente(8);

			SDL_Surface *titleb = TTF_RenderText_Solid(fuente, i->getContainer(contenedor)->getBoton(bon)->getTitulo(), fuentec);
			destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getBoton(bon)->y+(i->getContainer(contenedor)->getBoton(bon)->alto/2)-(titleb->h/2);
			destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getBoton(bon)->x+(i->getContainer(contenedor)->getBoton(bon)->ancho/2)-(titleb->w/2);
			destino.w = titleb->w;
			destino.h = titleb->h;
			SDL_BlitSurface(titleb, NULL, bbuffer, &destino);
			SDL_FreeSurface(titleb);
		}

		// Imagenes
		for(img = 0; i->getContainer(contenedor)->getImagen(img) != NULL; img++) {
			if(!i->getContainer(contenedor)->getImagen(img)->isDinamic()) {
				if(i->getContainer(contenedor)->getImagen(img)->img != NULL) {
					destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getImagen(img)->y;
					destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getImagen(img)->x;
					destino.w = i->getContainer(contenedor)->getImagen(img)->img->w;
					destino.h = i->getContainer(contenedor)->getImagen(img)->img->h;
					SDL_BlitSurface(i->getContainer(contenedor)->getImagen(img)->img, NULL, bbuffer, &destino);
				} else {
					Error::Log((char *)"No hay imagen para dibujar...", 1);
				}
			} else {
				string toload = i->getContainer(contenedor)->getImagen(img)->inicio;
				toload.append(i->getContainer(contenedor)->getSelector(i->getContainer(contenedor)->getImagen(img)->selector)->getSelected());
				toload.append(i->getContainer(contenedor)->getImagen(img)->final);
				if(i->getContainer(contenedor)->getImagen(img)->currentpath.compare(toload) != 0) {
					SDL_FreeSurface(i->getContainer(contenedor)->getImagen(img)->img);
					i->getContainer(contenedor)->getImagen(img)->img = IMG_Load(toload.c_str());
				}
				destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getImagen(img)->y;
				destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getImagen(img)->x;
				destino.w = i->getContainer(contenedor)->getImagen(img)->img->w;
				destino.h = i->getContainer(contenedor)->getImagen(img)->img->h;

				SDL_BlitSurface(i->getContainer(contenedor)->getImagen(img)->img, NULL, bbuffer, &destino);
			}
		}

		// Selectores
		for(int sel = 0; i->getContainer(contenedor)->getSelector(sel) != NULL; sel++) {
			origen.x = 4;
			origen.y = 26;
			origen.w = 9;
			origen.h = 14;

			destino.x = i->getContainer(contenedor)->x+i->getContainer(contenedor)->getSelector(sel)->x;
			destino.y = i->getContainer(contenedor)->y+i->getContainer(contenedor)->getSelector(sel)->y;
			destino.w = 9;
			destino.h = 14;

			SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

			origen.w = 1;
			destino.x += 12;
			destino.w = 1;
			origen.x = 1;

			SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
			destino.x++;
			origen.x--;

			if(8 != this->tam_fuente) this->AbrirFuente(8);
			SDL_Surface *seleccionado = TTF_RenderText_Solid(fuente, i->getContainer(contenedor)->getSelector(sel)->getSelected().c_str(), fuentec);
			i->getContainer(contenedor)->getSelector(sel)->ancho = seleccionado->w+8;

			for(int an = 0; an < seleccionado->w+8; an++) {
				SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
				destino.x++;
			}

			origen.x++;
			SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

			destino.w = 9;
			destino.x += 4;
			origen.x = 13;
			origen.w = 9;

			SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

			destino.x -= 8+seleccionado->w;
			destino.y += 3;
			destino.w = seleccionado->w;

			SDL_BlitSurface(seleccionado, NULL, bbuffer, &destino);

			SDL_FreeSurface(seleccionado);
		}
	}
}

void Graficos::DibujarJugadores() {
	int tilexinic = player->x-16;
	int tileyinic = player->y-12;
	if(this->tam_fuente != 8) this->AbrirFuente(8);

	SDL_Rect destino, origen;
	origen.w = 32;
	origen.h = 32;

	destino.w = 32;
	destino.h = 32;

	// Dibuja el resto de personajes en relación a tu personaje
	for(int x = 0; x < game->jugadores(); x++) {
	    Jugador *jd = game->getJugadorByIndex(x);
		if(abs(player->x-jd->x) < 17 && abs(player->y-jd->y) < 13) {
			origen.x = 32*jd->spriteState;
			origen.y = 32*(jd->traje-1);

			destino.x = ((jd->x-tilexinic)*32);
			destino.y = ((jd->y-tileyinic)*32);

			if(jd->h_offset > 0) destino.x = ((jd->x+1-tilexinic)*32)-jd->h_offset;
			else if(jd->h_offset < 0) destino.x = ((jd->x-1-tilexinic)*32)-jd->h_offset;
			else if(jd->v_offset > 0) destino.y = ((jd->y+1-tileyinic)*32)-jd->v_offset;
			else if(jd->v_offset < 0) destino.y = ((jd->y-1-tileyinic)*32)-jd->v_offset;

			if(player->h_offset > 0) destino.x -= 32;
			else if(player->h_offset < 0) destino.x += 32;
			else if(player->v_offset > 0) destino.y -= 32;
			else if(player->v_offset < 0) destino.y += 32;

			destino.x += player->h_offset;
			destino.y += player->v_offset;

			SDL_BlitSurface(personajes, &origen, bbuffer, &destino);
			// Dibujar etiqueta del nombre del resto de personajes
			SDL_Surface *lblname = TTF_RenderText_Solid(fuente, (char *)jd->nombre.c_str(), fuentec);

			destino.x = (destino.x+16)-(lblname->w/2);
			destino.y = destino.y+36;
			destino.w = lblname->w;
			destino.h = lblname->h;

			SDL_BlitSurface(lblname, NULL, bbuffer, &destino);
			SDL_FreeSurface(lblname);
		}
	}

	// Dibuja tu personaje
	destino.x = 1024/2;
	destino.y = 768/2;
	destino.w = 32;
	destino.h = 32;

	origen.x = 32*player->spriteState;
	origen.y = 32*(player->traje-1);

	origen.w = 32;
	origen.h = 32;

	SDL_BlitSurface(personajes, &origen, bbuffer, &destino);

	// Dibuja etiqueta de tu personaje
	SDL_Surface *nombre = TTF_RenderText_Solid(fuente, (char *)player->nombre.c_str(), fuentec);
	destino.x = 528-(nombre->w/2);
	destino.y = 420;
	destino.w = nombre->w;
	destino.h = nombre->h;

	SDL_BlitSurface(nombre, NULL, bbuffer, &destino);
	SDL_FreeSurface(nombre);
}

void Graficos::Volcar() {
	SDL_BlitSurface(bbuffer, NULL, pantalla, NULL);
	SDL_Flip(pantalla);
}

void Graficos::AbrirFuente(int tamano) {
	TTF_CloseFont(fuente);
	fuente = TTF_OpenFont((path+"data/pixel_font.ttf").c_str(), tamano);
	if(!fuente) {
   		Error::Log(TTF_GetError(), 2);
	}
	this->tam_fuente = tamano;
}
void Graficos::DibujarMensaje(char *msg, Jugador *jd)
{
	SDL_Rect destino, origen;
	int tilexinic = player->x-16;
	int tileyinic = player->y-12;
	if(this->tam_fuente != 8) this->AbrirFuente(8);

	if(abs(player->x-jd->x) < 17 && abs(player->y-jd->y) < 13) {
		origen.y = 41;
		origen.w = 28;
		origen.h = 23;

		if(jd->mensajesMostrados == 0) {
			origen.x = 0;
		} else {
			origen.x = 32;
		}

		destino.x = (jd->x-tilexinic)*32;
		destino.y = ((jd->y-tileyinic)*32)-25-jd->mensajesMostrados*20;

		if(jd->x != player->x && jd->y != player->y) {
			if(jd->h_offset < 0) destino.x = ((jd->x-1-tilexinic)*32)-jd->h_offset;
			else if(jd->h_offset > 0) destino.x = ((jd->x+1-tilexinic)*32)-jd->h_offset;
			else if(jd->v_offset < 0) destino.y = ((jd->y-1-tileyinic)*32)-jd->v_offset-25-jd->mensajesMostrados*20;
			else if(jd->v_offset > 0) destino.y = ((jd->y+1-tileyinic)*32)-jd->v_offset-25-jd->mensajesMostrados*20;

			if(player->h_offset > 0) destino.x -= 32;
			else if(player->h_offset < 0) destino.x += 32;
			else if(player->v_offset > 0) destino.y -= 32;
			else if(player->v_offset < 0) destino.y += 32;

			destino.x += player->h_offset;
			destino.y += player->v_offset;
		}

		destino.w = 28;
		destino.h = 23;

		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

		origen.x = 27;
		origen.w = 1;
		origen.h = 14;

		destino.x += 27;
		destino.w = 1;
		destino.h = 14;

		SDL_Surface *mensaje = TTF_RenderText_Solid(fuente, msg, fuentec);

		for(int y = 0; y < mensaje->w+8; y++) {
			destino.x++;
			SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);
		}

		origen.x = 29;
		origen.y = 41;
		origen.w = 4;
		origen.h = 14;

		destino.w = 4;

		SDL_BlitSurface(gui_images, &origen, bbuffer, &destino);

		destino.y += 3;
		destino.x = (jd->x-tilexinic)*32+32;
		if(jd->x != player->x && jd->y != player->y) {
			if(jd->h_offset < 0) destino.x = ((jd->x-1-tilexinic)*32)-jd->h_offset+32;
			else if(jd->h_offset > 0) destino.x = ((jd->x+1-tilexinic)*32)-jd->h_offset+32;
			else if(jd->v_offset < 0) destino.y = ((jd->y-1-tileyinic)*32)-jd->v_offset-25-jd->mensajesMostrados*20+3;
			else if(jd->v_offset > 0) destino.y = ((jd->y+1-tileyinic)*32)-jd->v_offset-25-jd->mensajesMostrados*20+3;

			if(player->h_offset > 0) destino.x -= 32;
			else if(player->h_offset < 0) destino.x += 32;

			destino.x += player->h_offset;
		}
		destino.w = mensaje->w;
		destino.h = mensaje->h;

		SDL_BlitSurface(mensaje, NULL, bbuffer, &destino);
		SDL_FreeSurface(mensaje);
	}
}
