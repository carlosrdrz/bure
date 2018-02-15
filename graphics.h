#pragma once

#include "game.h"
#include "config.h"
#include "game_map.h"
#include "user_interface.h"
#include "player.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <list>

using namespace std;

class graphics {
public:
	explicit graphics(std::string basePath);
	~graphics();

    void draw(game &g);
	void draw(int tile, int x, int y, int h, int v, bool fullLayer, bool layer);
    // todo: no reason these are not const
	void draw(game_map *m, player &p, bool layer);
	void draw(user_interface *i);
    // todo: no reason these are not const
	void draw(player &p);

	void renderBackground();

	void openFont(int size);
	void clean();
	void flipBuffer();

private:
	std::string basePath;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *screenSurface, *tilesSurface, *playersSurface, *elementsSurface, *uiImagesSurface, *bgSurface;
    SDL_Texture *tilesTexture, *playersTexture, *elementsTexture, *uiImagesTexture, *bgTexture;
    SDL_Color backgroundColor, fontColor;
	TTF_Font *font;
	int fontSize;
};
