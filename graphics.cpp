#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "map.h"
#include "error.h"
#include "user_interface.h"
#include "player.h"
#include "game.h"
#include "graphics.h"
#include "config.h"

#define TILE_SIZE 32

extern user_interface *uiInstance;
extern game *gameInstance;
extern player *playerInstance;
extern config *configInstance;
extern std::string path;

graphics::graphics() {
    // Init video at 1024x768
    Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
    if (configInstance->getBoolValueOf("fullscreen")) flags |= SDL_WINDOW_OPENGL;
    window = SDL_CreateWindow("Bure Online 2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1440, flags);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_RenderSetScale(renderer, 2, 2);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Load the file tiles
    tilesSurface = IMG_Load((path + "data/tiles.png").c_str());
    if (tilesSurface == nullptr) Error::Log(SDL_GetError(), 2);
    tilesTexture = SDL_CreateTextureFromSurface(renderer, tilesSurface);

    playersSurface = IMG_Load((path + "data/personajes.png").c_str());
    if (playersSurface == nullptr) Error::Log(SDL_GetError(), 2);
    playersTexture = SDL_CreateTextureFromSurface(renderer, playersSurface);

    elementsSurface = IMG_Load((path + "data/elements.png").c_str());
    if (elementsSurface == nullptr) Error::Log(SDL_GetError(), 2);
    elementsTexture = SDL_CreateTextureFromSurface(renderer, elementsSurface);

    uiImagesSurface = IMG_Load((path + "data/gui.png").c_str());
    if (uiImagesSurface == nullptr) Error::Log(SDL_GetError(), 2);
    uiImagesTexture = SDL_CreateTextureFromSurface(renderer, uiImagesSurface);
    SDL_SetTextureBlendMode(uiImagesTexture, SDL_BLENDMODE_BLEND);

    // Init SDL_ttf and load font
    if (TTF_Init() < 0) Error::Log(SDL_GetError(), 2);

    font = TTF_OpenFont((path + "data/pixel_font.ttf").c_str(), 16);
    if (!font) Error::Log(TTF_GetError(), 2);

    fontSize = 16;

    fontColor.r = 255;
    fontColor.g = 255;
    fontColor.b = 255;
    backgroundColor.r = 114;
    backgroundColor.g = 107;
    backgroundColor.b = 82;

    // Load background
    bgSurface = IMG_Load((path + "data/background.jpg").c_str());
    if (bgSurface == nullptr) Error::Log(SDL_GetError(), 2);
    bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
}

graphics::~graphics() {
    SDL_DestroyTexture(tilesTexture);
    SDL_DestroyTexture(uiImagesTexture);
    SDL_DestroyTexture(playersTexture);
    SDL_DestroyTexture(elementsTexture);
    SDL_DestroyTexture(bgTexture);

    SDL_FreeSurface(tilesSurface);
    SDL_FreeSurface(uiImagesSurface);
    SDL_FreeSurface(playersSurface);
    SDL_FreeSurface(elementsSurface);
    SDL_FreeSurface(bgSurface);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void graphics::clean() {
    SDL_RenderClear(renderer);
}

void graphics::draw(int tile, int x, int y, int h, int v, bool fullLayer, bool layer = false) {
    SDL_Rect src, dst;

    int tilex = 0;
    int tiley = 0;
    if (!layer) {
        while (tile > 7) {
            tile -= 7;
            tiley += 32;
        }
        tilex = (tile - 1) * 32;
    } else {
        while (tile > 8) {
            tile -= 8;
            tiley += 32;
        }
        tilex = (tile - 1) * 32;
    }

    if (fullLayer) {
        src.x = tilex;
        src.y = tiley;
        src.w = TILE_SIZE;
        src.h = TILE_SIZE;

        dst.x = x + h;
        dst.y = y + v;
        dst.w = TILE_SIZE;
        dst.h = TILE_SIZE;
    } else {
        if (h > 0) {
            src.x = tilex + 32 - h;
            src.y = tiley;
            src.w = TILE_SIZE - (32 - h);
            src.h = TILE_SIZE;

            dst.x = x;
            dst.y = y;
            dst.w = TILE_SIZE - (32 - h);
            dst.h = TILE_SIZE;
        } else if (h < 0) {
            src.x = tilex;
            src.y = tiley;
            src.w = h;
            src.h = TILE_SIZE;

            dst.x = x;
            dst.y = y;
            dst.w = h;
            dst.h = TILE_SIZE;
        } else if (v > 0) {
            src.x = tilex;
            src.y = tiley + (32 - v);
            src.w = TILE_SIZE;
            src.h = v;

            dst.x = x;
            dst.y = y;
            dst.w = TILE_SIZE;
            dst.h = v;
        } else if (v < 0) {
            src.x = tilex;
            src.y = tiley;
            src.w = TILE_SIZE;
            src.h = 32 - v;

            dst.x = x;
            dst.y = y;
            dst.w = TILE_SIZE;
            dst.h = v;
        }
    }

    if (!layer) SDL_RenderCopy(renderer, tilesTexture, &src, &dst);
    else SDL_RenderCopy(renderer, elementsTexture, &src, &dst);
}

void graphics::draw(map *m, bool layer) {
    int empezar_x, empezar_y = 0;
    int jug_y = playerInstance->y;
    int jug_x = playerInstance->x;
    int x, y;

    playerInstance->moviendo = false;

    if (playerInstance->h_offset > 0) {
        empezar_y = 0;
        for (y = (jug_y - 12); y < (jug_y + 12); y++) {
            if (y >= 0 && y < m->height) {
                if (!layer)
                    draw(m->mapa[(jug_x - 16) + y * m->width], 0, empezar_y, playerInstance->h_offset, playerInstance->v_offset, false);
                else
                    draw(m->elementos[(jug_x - 16) + y * m->width], 0, empezar_y, playerInstance->h_offset, playerInstance->v_offset,
                         false, true);
            }
            empezar_y += TILE_SIZE;
        }
        jug_x++;
    }

    if (playerInstance->h_offset < 0) {
        empezar_y = 0;
        for (y = (jug_y - 12); y < (jug_y + 12); y++) {
            if (y >= 0 && y < m->height) {
                if (!layer)
                    draw(m->mapa[(jug_x + 15) + y * m->width], 1024 + playerInstance->h_offset, empezar_y, playerInstance->h_offset,
                         playerInstance->v_offset, false);
                else
                    draw(m->elementos[(jug_x + 15) + y * m->width], 1024 + playerInstance->h_offset, empezar_y,
                         playerInstance->h_offset, playerInstance->v_offset, false, true);
            }
            empezar_y += TILE_SIZE;
        }
        jug_x--;
    }
    if (playerInstance->v_offset > 0) {
        empezar_x = 0;
        for (x = (jug_x - 16); x < (jug_x + 16); x++) {
            if (x >= 0 && x < m->width) {
                if (!layer)
                    draw(m->mapa[(x + ((jug_y - 12) * m->width))], empezar_x, 0, playerInstance->h_offset, playerInstance->v_offset,
                         false);
                else
                    draw(m->elementos[(x + ((jug_y - 12) * m->width))], empezar_x, 0, playerInstance->h_offset,
                         playerInstance->v_offset, false, true);
            }
            empezar_x += TILE_SIZE;
        }
        jug_y++;
    }
    if (playerInstance->v_offset < 0) {
        empezar_x = 0;
        for (x = (jug_x - 16); x < (jug_x + 16); x++) {
            if (x >= 0 && x < m->width) {
                if (!layer)
                    draw(m->mapa[(x + ((jug_y + 11) * m->width))], empezar_x, 768 + playerInstance->v_offset, playerInstance->h_offset,
                         playerInstance->v_offset, false);
                else
                    draw(m->elementos[(x + ((jug_y + 11) * m->width))], empezar_x, 768 + playerInstance->v_offset,
                         playerInstance->h_offset, playerInstance->v_offset, false, true);
            }
            empezar_x += TILE_SIZE;
        }
        jug_y--;
    }

    empezar_x = empezar_y = 0;

    for (y = (jug_y - 12); y < (jug_y + 12); y++) {
        for (x = (jug_x - 16); x < (jug_x + 16); x++) {
            if (x >= 0 && y >= 0 && x < m->width && y < m->height) {
                if (!layer)
                    draw(m->mapa[(x + (y * m->width))], empezar_x, empezar_y, playerInstance->h_offset, playerInstance->v_offset, true);
                else
                    draw(m->elementos[(x + (y * m->width))], empezar_x, empezar_y, playerInstance->h_offset, playerInstance->v_offset,
                         true, true);
            }
            empezar_x += TILE_SIZE;
        }
        empezar_x = 0;
        empezar_y += TILE_SIZE;
    }
}

void graphics::renderBackground() {
    SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
}

void graphics::draw(user_interface *i) {
    SDL_Rect dst, src;

    int containerIndex, lb, ib, bon, img;
    for (containerIndex = 0; i->getContainer(containerIndex) != nullptr; containerIndex++) {
        if (!i->getContainer(containerIndex)->invisible) {
            // Globales para todo el contenedor
            src.w = 1;
            src.h = 26;
            src.y = 0;

            // Fondo del contenedor
            dst.x = i->getContainer(containerIndex)->x;
            dst.y = i->getContainer(containerIndex)->y;
            dst.w = i->getContainer(containerIndex)->width;
            dst.h = i->getContainer(containerIndex)->height;

            SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &dst);

            // Barras horizontales
            dst.w = 1;
            dst.h = 26;

            src.x = 104;
            dst.y -= 12;
            dst.x--;
            for (int y = 0; y < i->getContainer(containerIndex)->width; y++) {
                dst.x++;
                SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
            }

            dst.y += i->getContainer(containerIndex)->height;
            dst.x = i->getContainer(containerIndex)->x - 1;
            for (int y = 0; y < i->getContainer(containerIndex)->width; y++) {
                dst.x++;
                SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
            }

            // Barras verticales
            src.x = 130;
            src.w = 26;
            src.h = 1;

            dst.x = i->getContainer(containerIndex)->x - 11;
            dst.y = i->getContainer(containerIndex)->y;
            dst.h = 1;
            dst.w = 26;

            for (int y = 0; y < i->getContainer(containerIndex)->height; y++) {
                dst.y++;
                SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
            }

            dst.x += i->getContainer(containerIndex)->width - 4;
            dst.y = i->getContainer(containerIndex)->y;
            for (int y = 0; y < i->getContainer(containerIndex)->height; y++) {
                dst.y++;
                SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
            }

            // Todas las esquinas
            src.w = 26;
            src.h = 26;
            dst.w = 26;
            dst.h = 26;

            src.x = 0;
            src.y = 0;
            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->width - 15;
            dst.y = i->getContainer(containerIndex)->y - 12;
            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            src.x += 26;
            dst.x -= i->getContainer(containerIndex)->width - 4;
            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            src.x += 26;
            dst.y += i->getContainer(containerIndex)->height;
            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->width - 15;
            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            src.x += 26;
            dst.x -= i->getContainer(containerIndex)->width - 4;
            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
        }

        // Labels
        for (lb = 0; i->getContainer(containerIndex)->getLabel(lb) != nullptr; lb++) {
            if (i->getContainer(containerIndex)->getLabel(lb)->getTamano() != this->fontSize) {
                this->openFont(i->getContainer(containerIndex)->getLabel(lb)->getTamano());
            }

            SDL_Surface *textSurface = TTF_RenderText_Solid(font, i->getContainer(containerIndex)->getLabel(lb)->getText(),
                                                      fontColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            if (textSurface != nullptr) {
                dst.x = i->getContainer(containerIndex)->getLabel(lb)->x + i->getContainer(containerIndex)->x;
                dst.y = i->getContainer(containerIndex)->getLabel(lb)->y + i->getContainer(containerIndex)->y;
                dst.w = textSurface->w;
                dst.h = textSurface->h;

                SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }

        // InputBoxes
        for (ib = 0; i->getContainer(containerIndex)->getIB(ib) != nullptr; ++ib) {
            // Se dibuja el input box
            src.x = (i->getContainer(containerIndex)->getIB(ib)->getFocus() && i->getContainer(containerIndex)->getFocus() &&
                     !i->blocked) ? 1 : 3;
            src.y = 26;
            src.w = 1;
            src.h = 14;

            dst.x = i->getContainer(containerIndex)->getIB(ib)->x + i->getContainer(containerIndex)->x;
            dst.y = i->getContainer(containerIndex)->getIB(ib)->y + i->getContainer(containerIndex)->y;
            dst.w = 1;
            dst.h = 14;

            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
            dst.x++;
            src.x--;
            for (int an = 0; an < i->getContainer(containerIndex)->getIB(ib)->width - 2; an++) {
                SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
                dst.x++;
            }
            src.x++;
            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            // La etiqueta del inputbox
            if (8 != this->fontSize) {
                this->openFont(8);
            }

            SDL_Surface *titleSurface = TTF_RenderText_Solid(font, i->getContainer(containerIndex)->getIB(ib)->getTitulo(),
                                                      fontColor);
            SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

            dst.x = i->getContainer(containerIndex)->getIB(ib)->x + i->getContainer(containerIndex)->x - titleSurface->w - 5;
            dst.y += 3;
            dst.w = titleSurface->w;
            dst.h = titleSurface->h;

            SDL_RenderCopy(renderer, titleTexture, nullptr, &dst);

            if (i->getContainer(containerIndex)->getIB(ib)->getCaracteres() != 0) {
                SDL_Surface *textSurface;
                if (i->getContainer(containerIndex)->getIB(ib)->secreto == 1) {
                    char sec[13];
                    for (int sd = 0; sd < i->getContainer(containerIndex)->getIB(ib)->getCaracteres(); sd++) {
                        sec[sd] = 42;
                    }
                    sec[i->getContainer(containerIndex)->getIB(ib)->getCaracteres()] = 0;
                    textSurface = TTF_RenderText_Solid(font, sec, fontColor);
                } else {
                    textSurface = TTF_RenderText_Solid(font, i->getContainer(containerIndex)->getIB(ib)->getTexto(), fontColor);
                }
                dst.x += titleSurface->w + 10;
                dst.w = std::min(i->getContainer(containerIndex)->getIB(ib)->width, textSurface->w);

                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
                SDL_DestroyTexture(textTexture);
                SDL_FreeSurface(textSurface);
            }

            SDL_DestroyTexture(titleTexture);
            SDL_FreeSurface(titleSurface);
        }

        // Botones
        for (bon = 0; i->getContainer(containerIndex)->getBoton(bon) != nullptr; bon++) {
            if (i->getContainer(containerIndex)->getBoton(bon)->press == 0) SDL_SetRenderDrawColor(renderer, 63, 63, 63, SDL_ALPHA_OPAQUE);
            else SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getBoton(bon)->x;
            dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getBoton(bon)->y;
            dst.w = i->getContainer(containerIndex)->getBoton(bon)->width;
            dst.h = i->getContainer(containerIndex)->getBoton(bon)->height;

            SDL_RenderFillRect(renderer, &dst);

            if (i->getContainer(containerIndex)->getBoton(bon)->press == 0) SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            else SDL_SetRenderDrawColor(renderer, 63, 63, 63, SDL_ALPHA_OPAQUE);

            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getBoton(bon)->x;
            dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getBoton(bon)->y;
            dst.w = i->getContainer(containerIndex)->getBoton(bon)->width;
            dst.h = 1;

            SDL_RenderFillRect(renderer, &dst);

            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getBoton(bon)->x;
            dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getBoton(bon)->y +
                    i->getContainer(containerIndex)->getBoton(bon)->height;
            dst.w = i->getContainer(containerIndex)->getBoton(bon)->width;
            dst.h = 1;

            SDL_RenderFillRect(renderer, &dst);

            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getBoton(bon)->x;
            dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getBoton(bon)->y;
            dst.w = 1;
            dst.h = i->getContainer(containerIndex)->getBoton(bon)->height;

            SDL_RenderFillRect(renderer, &dst);

            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getBoton(bon)->x +
                    i->getContainer(containerIndex)->getBoton(bon)->width;
            dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getBoton(bon)->y;
            dst.w = 1;
            dst.h = i->getContainer(containerIndex)->getBoton(bon)->height;

            SDL_RenderFillRect(renderer, &dst);

            // Dibujar la etiqueta
            if (this->fontSize != 8) this->openFont(8);

            SDL_Surface *titlebSurface = TTF_RenderText_Solid(font, i->getContainer(containerIndex)->getBoton(bon)->getTitulo(),
                                                       fontColor);
            dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getBoton(bon)->y +
                    (i->getContainer(containerIndex)->getBoton(bon)->height / 2) - (titlebSurface->h / 2);
            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getBoton(bon)->x +
                    (i->getContainer(containerIndex)->getBoton(bon)->width / 2) - (titlebSurface->w / 2);
            dst.w = titlebSurface->w;
            dst.h = titlebSurface->h;

            SDL_Texture *titlebTexture = SDL_CreateTextureFromSurface(renderer, titlebSurface);
            SDL_RenderCopy(renderer, titlebTexture, nullptr, &dst);
            SDL_DestroyTexture(titlebTexture);
            SDL_FreeSurface(titlebSurface);
        }

        // Imagenes
        for (img = 0; i->getContainer(containerIndex)->getImagen(img) != nullptr; img++) {
            if (!i->getContainer(containerIndex)->getImagen(img)->isDinamic()) {
                if (i->getContainer(containerIndex)->getImagen(img)->img != nullptr) {
                    dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getImagen(img)->y;
                    dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getImagen(img)->x;
                    dst.w = i->getContainer(containerIndex)->getImagen(img)->img->w;
                    dst.h = i->getContainer(containerIndex)->getImagen(img)->img->h;
                    // TODO: Fix this images thing
                    // SDL_BlitSurface(i->getContainer(containerIndex)->getImagen(img)->img, NULL, bbuffer, &dst);
                } else {
                    Error::Log((char *) "No hay imagen para dibujar...", 1);
                }
            } else {
                string toload = i->getContainer(containerIndex)->getImagen(img)->inicio;
                toload.append(i->getContainer(containerIndex)->getSelector(
                        i->getContainer(containerIndex)->getImagen(img)->selector)->getSelected());
                toload.append(i->getContainer(containerIndex)->getImagen(img)->final);
                if (i->getContainer(containerIndex)->getImagen(img)->currentpath.compare(toload) != 0) {
                    SDL_FreeSurface(i->getContainer(containerIndex)->getImagen(img)->img);
                    i->getContainer(containerIndex)->getImagen(img)->img = IMG_Load(toload.c_str());
                }
                dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getImagen(img)->y;
                dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getImagen(img)->x;
                dst.w = i->getContainer(containerIndex)->getImagen(img)->img->w;
                dst.h = i->getContainer(containerIndex)->getImagen(img)->img->h;

                // TODO: Fix this images thing
                // SDL_BlitSurface(i->getContainer(containerIndex)->getImagen(img)->img, NULL, bbuffer, &dst);
            }
        }

        // Selectores
        for (int sel = 0; i->getContainer(containerIndex)->getSelector(sel) != nullptr; sel++) {
            src.x = 4;
            src.y = 26;
            src.w = 9;
            src.h = 14;

            dst.x = i->getContainer(containerIndex)->x + i->getContainer(containerIndex)->getSelector(sel)->x;
            dst.y = i->getContainer(containerIndex)->y + i->getContainer(containerIndex)->getSelector(sel)->y;
            dst.w = 9;
            dst.h = 14;

            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            src.w = 1;
            dst.x += 12;
            dst.w = 1;
            src.x = 1;

            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            dst.x++;
            src.x--;

            if (8 != this->fontSize) this->openFont(8);
            SDL_Surface *seleccionado = TTF_RenderText_Solid(font, i->getContainer(containerIndex)->getSelector(
                    sel)->getSelected().c_str(), fontColor);
            i->getContainer(containerIndex)->getSelector(sel)->width = seleccionado->w + 8;

            for (int an = 0; an < seleccionado->w + 8; an++) {
                SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
                dst.x++;
            }

            src.x++;
            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            dst.w = 9;
            dst.x += 4;
            src.x = 13;
            src.w = 9;

            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

            dst.x -= 8 + seleccionado->w;
            dst.y += 3;
            dst.w = seleccionado->w;
            dst.h = seleccionado->h;

            SDL_Texture *seleccionadoTexture = SDL_CreateTextureFromSurface(renderer, seleccionado);
            SDL_RenderCopy(renderer, seleccionadoTexture, nullptr, &dst);

            SDL_DestroyTexture(seleccionadoTexture);
            SDL_FreeSurface(seleccionado);
        }
    }
}

void graphics::drawPlayers() {
    int tilexinic = playerInstance->x - 16;
    int tileyinic = playerInstance->y - 12;
    if (this->fontSize != 8) this->openFont(8);

    SDL_Rect dst, src;
    src.w = 32;
    src.h = 32;

    dst.w = 32;
    dst.h = 32;

    // Dibuja el resto de personajes en relación a tu personaje
    for (int x = 0; x < gameInstance->jugadores(); x++) {
        player *jd = gameInstance->getPlayerByIndex(x);
        if (abs(playerInstance->x - jd->x) < 17 && abs(playerInstance->y - jd->y) < 13) {
            src.x = 32 * jd->spriteState;
            src.y = 32 * (jd->traje - 1);

            dst.x = ((jd->x - tilexinic) * 32);
            dst.y = ((jd->y - tileyinic) * 32);

            if (jd->h_offset > 0) dst.x = ((jd->x + 1 - tilexinic) * 32) - jd->h_offset;
            else if (jd->h_offset < 0) dst.x = ((jd->x - 1 - tilexinic) * 32) - jd->h_offset;
            else if (jd->v_offset > 0) dst.y = ((jd->y + 1 - tileyinic) * 32) - jd->v_offset;
            else if (jd->v_offset < 0) dst.y = ((jd->y - 1 - tileyinic) * 32) - jd->v_offset;

            if (playerInstance->h_offset > 0) dst.x -= 32;
            else if (playerInstance->h_offset < 0) dst.x += 32;
            else if (playerInstance->v_offset > 0) dst.y -= 32;
            else if (playerInstance->v_offset < 0) dst.y += 32;

            dst.x += playerInstance->h_offset;
            dst.y += playerInstance->v_offset;

            SDL_RenderCopy(renderer, playersTexture, &src, &dst);

            // Dibujar etiqueta del nombre del resto de personajes
            SDL_Surface *lblname = TTF_RenderText_Solid(font, (char *) jd->nombre.c_str(), fontColor);
            SDL_Texture *lblnameTexture = SDL_CreateTextureFromSurface(renderer, lblname);

            dst.x = (dst.x + 16) - (lblname->w / 2);
            dst.y = dst.y + 36;
            dst.w = lblname->w;
            dst.h = lblname->h;

            SDL_RenderCopy(renderer, lblnameTexture, nullptr, &dst);

            SDL_DestroyTexture(lblnameTexture);
            SDL_FreeSurface(lblname);
        }
    }

    // Dibuja tu personaje
    dst.x = 1024 / 2;
    dst.y = 768 / 2;
    dst.w = 32;
    dst.h = 32;

    src.x = 32 * playerInstance->spriteState;
    src.y = 32 * (playerInstance->traje - 1);

    src.w = 32;
    src.h = 32;

    SDL_RenderCopy(renderer, playersTexture, &src, &dst);

    // Dibuja etiqueta de tu personaje
    SDL_Surface *nombre = TTF_RenderText_Solid(font, (char *) playerInstance->nombre.c_str(), fontColor);
    SDL_Texture *nombreTexture = SDL_CreateTextureFromSurface(renderer, nombre);

    dst.x = 528 - (nombre->w / 2);
    dst.y = 420;
    dst.w = nombre->w;
    dst.h = nombre->h;

    SDL_RenderCopy(renderer, nombreTexture, nullptr, &dst);

    SDL_DestroyTexture(nombreTexture);
    SDL_FreeSurface(nombre);
}

void graphics::flipBuffer() {
    SDL_RenderPresent(renderer);
}

void graphics::openFont(int size) {
    TTF_CloseFont(font);
    font = TTF_OpenFont((path + "data/pixel_font.ttf").c_str(), size);
    if (!font) {
        Error::Log(TTF_GetError(), 2);
    }
    this->fontSize = size;
}

void graphics::drawMessage(char *msg, player *jd) {
    SDL_Rect dst, src;
    int tilexinic = playerInstance->x - 16;
    int tileyinic = playerInstance->y - 12;
    if (this->fontSize != 8) this->openFont(8);

    if (abs(playerInstance->x - jd->x) < 17 && abs(playerInstance->y - jd->y) < 13) {
        src.y = 41;
        src.w = 28;
        src.h = 23;

        if (jd->mensajesMostrados == 0) {
            src.x = 0;
        } else {
            src.x = 32;
        }

        dst.x = (jd->x - tilexinic) * 32;
        dst.y = ((jd->y - tileyinic) * 32) - 25 - jd->mensajesMostrados * 20;

        if (jd->x != playerInstance->x && jd->y != playerInstance->y) {
            if (jd->h_offset < 0) dst.x = ((jd->x - 1 - tilexinic) * 32) - jd->h_offset;
            else if (jd->h_offset > 0) dst.x = ((jd->x + 1 - tilexinic) * 32) - jd->h_offset;
            else if (jd->v_offset < 0) dst.y = ((jd->y - 1 - tileyinic) * 32) - jd->v_offset - 25 -
                                                   jd->mensajesMostrados * 20;
            else if (jd->v_offset > 0) dst.y = ((jd->y + 1 - tileyinic) * 32) - jd->v_offset - 25 -
                                                   jd->mensajesMostrados * 20;

            if (playerInstance->h_offset > 0) dst.x -= 32;
            else if (playerInstance->h_offset < 0) dst.x += 32;
            else if (playerInstance->v_offset > 0) dst.y -= 32;
            else if (playerInstance->v_offset < 0) dst.y += 32;

            dst.x += playerInstance->h_offset;
            dst.y += playerInstance->v_offset;
        }

        dst.w = 28;
        dst.h = 23;

        SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

        src.x = 27;
        src.w = 1;
        src.h = 14;

        dst.x += 27;
        dst.w = 1;
        dst.h = 14;

        SDL_Surface *mensaje = TTF_RenderText_Solid(font, msg, fontColor);
        SDL_Texture *msgTexture = SDL_CreateTextureFromSurface(renderer, mensaje);

        for (int y = 0; y < mensaje->w + 8; y++) {
            dst.x++;
            SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
        }

        src.x = 29;
        src.y = 41;
        src.w = 4;
        src.h = 14;

        dst.w = 4;

        SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

        dst.y += 3;
        dst.x = (jd->x - tilexinic) * 32 + 32;
        if (jd->x != playerInstance->x && jd->y != playerInstance->y) {
            if (jd->h_offset < 0) dst.x = ((jd->x - 1 - tilexinic) * 32) - jd->h_offset + 32;
            else if (jd->h_offset > 0) dst.x = ((jd->x + 1 - tilexinic) * 32) - jd->h_offset + 32;
            else if (jd->v_offset < 0) dst.y = ((jd->y - 1 - tileyinic) * 32) - jd->v_offset - 25 -
                                                   jd->mensajesMostrados * 20 + 3;
            else if (jd->v_offset > 0) dst.y = ((jd->y + 1 - tileyinic) * 32) - jd->v_offset - 25 -
                                                   jd->mensajesMostrados * 20 + 3;

            if (playerInstance->h_offset > 0) dst.x -= 32;
            else if (playerInstance->h_offset < 0) dst.x += 32;

            dst.x += playerInstance->h_offset;
        }
        dst.w = mensaje->w;
        dst.h = mensaje->h;

        SDL_RenderCopy(renderer, msgTexture, nullptr, &dst);

        SDL_DestroyTexture(msgTexture);
        SDL_FreeSurface(mensaje);
    }
}
