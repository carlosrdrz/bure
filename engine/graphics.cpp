#include <algorithm>

#include "graphics.h"
#include "utils/logger.h"

#define TILE_SIZE 32

namespace bure {
// todo: fix basepath
graphics::graphics(std::string basePath) : basePath(basePath) {
  atexit(SDL_Quit);

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    logger::error(SDL_GetError());
  }

  // Init video at 1024x768
  Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
  if (config::instance.getBoolValueOf("fullscreen")) flags |= SDL_WINDOW_OPENGL;
  window = SDL_CreateWindow("Bure", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, 1920, 1440, flags);
  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_RenderSetScale(renderer, 2, 2);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Load the file tiles
  tilesSurface = IMG_Load((basePath + "data/tiles.png").c_str());
  if (tilesSurface == nullptr) logger::error(SDL_GetError());
  tilesTexture = SDL_CreateTextureFromSurface(renderer, tilesSurface);

  playersSurface = IMG_Load((basePath + "data/personajes.png").c_str());
  if (playersSurface == nullptr) logger::error(SDL_GetError());
  playersTexture = SDL_CreateTextureFromSurface(renderer, playersSurface);

  elementsSurface = IMG_Load((basePath + "data/elements.png").c_str());
  if (elementsSurface == nullptr) logger::error(SDL_GetError());
  elementsTexture = SDL_CreateTextureFromSurface(renderer, elementsSurface);

  uiImagesSurface = IMG_Load((basePath + "data/gui.png").c_str());
  if (uiImagesSurface == nullptr) logger::error(SDL_GetError());
  uiImagesTexture = SDL_CreateTextureFromSurface(renderer, uiImagesSurface);
  SDL_SetTextureBlendMode(uiImagesTexture, SDL_BLENDMODE_BLEND);

  // Init SDL_ttf and load font
  if (TTF_Init() < 0) logger::error(SDL_GetError());

  font = TTF_OpenFont((basePath + "data/pixel_font.ttf").c_str(), 16);
  if (!font) logger::error(TTF_GetError());

  fontSize = 16;

  fontColor.r = 255;
  fontColor.g = 255;
  fontColor.b = 255;
  backgroundColor.r = 114;
  backgroundColor.g = 107;
  backgroundColor.b = 82;

  // Load background
  bgSurface = IMG_Load((basePath + "data/background.jpg").c_str());
  if (bgSurface == nullptr) logger::error(SDL_GetError());
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

void graphics::clean() { SDL_RenderClear(renderer); }

// void graphics::draw(game &g) {
//   draw(g.getMap(), g.getPlayer(), false);
//   draw(g.getPlayer());
//   draw(g.getMap(), g.getPlayer(), true);
// }

// void graphics::draw(int tile, int x, int y, int h, int v, bool fullLayer,
//                     bool layer = false) {
//   SDL_Rect src, dst;
//
//   int tilex = 0;
//   int tiley = 0;
//   if (!layer) {
//     while (tile > 7) {
//       tile -= 7;
//       tiley += 32;
//     }
//     tilex = (tile - 1) * 32;
//   } else {
//     while (tile > 8) {
//       tile -= 8;
//       tiley += 32;
//     }
//     tilex = (tile - 1) * 32;
//   }
//
//   if (fullLayer) {
//     src.x = tilex;
//     src.y = tiley;
//     src.w = TILE_SIZE;
//     src.h = TILE_SIZE;
//
//     dst.x = x + h;
//     dst.y = y + v;
//     dst.w = TILE_SIZE;
//     dst.h = TILE_SIZE;
//   } else {
//     if (h > 0) {
//       src.x = tilex + 32 - h;
//       src.y = tiley;
//       src.w = TILE_SIZE - (32 - h);
//       src.h = TILE_SIZE;
//
//       dst.x = x;
//       dst.y = y;
//       dst.w = TILE_SIZE - (32 - h);
//       dst.h = TILE_SIZE;
//     } else if (h < 0) {
//       src.x = tilex;
//       src.y = tiley;
//       src.w = h;
//       src.h = TILE_SIZE;
//
//       dst.x = x;
//       dst.y = y;
//       dst.w = h;
//       dst.h = TILE_SIZE;
//     } else if (v > 0) {
//       src.x = tilex;
//       src.y = tiley + (32 - v);
//       src.w = TILE_SIZE;
//       src.h = v;
//
//       dst.x = x;
//       dst.y = y;
//       dst.w = TILE_SIZE;
//       dst.h = v;
//     } else if (v < 0) {
//       src.x = tilex;
//       src.y = tiley;
//       src.w = TILE_SIZE;
//       src.h = 32 - v;
//
//       dst.x = x;
//       dst.y = y;
//       dst.w = TILE_SIZE;
//       dst.h = v;
//     }
//   }
//
//   if (!layer)
//     SDL_RenderCopy(renderer, tilesTexture, &src, &dst);
//   else
//     SDL_RenderCopy(renderer, elementsTexture, &src, &dst);
// }

// void graphics::draw(game_map *m, player &p, bool layer) {
//   int empezar_x, empezar_y = 0;
//   int jug_y = p.y;
//   int jug_x = p.x;
//   int x, y;
//
//   p.moviendo = false;
//
//   if (p.h_offset > 0) {
//     empezar_y = 0;
//     for (y = (jug_y - 12); y < (jug_y + 12); y++) {
//       if (y >= 0 && y < m->height) {
//         if (!layer)
//           draw(m->mapa[(jug_x - 16) + y * m->width],
//  0, empezar_y, p.h_offset,
//                p.v_offset, false);
//         else
//           draw(m->elementos[(jug_x - 16) + y * m->width], 0, empezar_y,
//                p.h_offset, p.v_offset, false, true);
//       }
//       empezar_y += TILE_SIZE;
//     }
//     jug_x++;
//   }
//
//   if (p.h_offset < 0) {
//     empezar_y = 0;
//     for (y = (jug_y - 12); y < (jug_y + 12); y++) {
//       if (y >= 0 && y < m->height) {
//         if (!layer)
//           draw(m->mapa[(jug_x + 15) + y * m->width], 1024 + p.h_offset,
//                empezar_y, p.h_offset, p.v_offset, false);
//         else
//           draw(m->elementos[(jug_x + 15) + y * m->width], 1024 + p.h_offset,
//                empezar_y, p.h_offset, p.v_offset, false, true);
//       }
//       empezar_y += TILE_SIZE;
//     }
//     jug_x--;
//   }
//   if (p.v_offset > 0) {
//     empezar_x = 0;
//     for (x = (jug_x - 16); x < (jug_x + 16); x++) {
//       if (x >= 0 && x < m->width) {
//         if (!layer)
//           draw(m->mapa[(x + ((jug_y - 12) * m->width))], empezar_x, 0,
//                p.h_offset, p.v_offset, false);
//         else
//           draw(m->elementos[(x + ((jug_y - 12) * m->width))], empezar_x, 0,
//                p.h_offset, p.v_offset, false, true);
//       }
//       empezar_x += TILE_SIZE;
//     }
//     jug_y++;
//   }
//   if (p.v_offset < 0) {
//     empezar_x = 0;
//     for (x = (jug_x - 16); x < (jug_x + 16); x++) {
//       if (x >= 0 && x < m->width) {
//         if (!layer)
//           draw(m->mapa[(x + ((jug_y + 11) * m->width))], empezar_x,
//                768 + p.v_offset, p.h_offset, p.v_offset, false);
//         else
//           draw(m->elementos[(x + ((jug_y + 11) * m->width))], empezar_x,
//                768 + p.v_offset, p.h_offset, p.v_offset, false, true);
//       }
//       empezar_x += TILE_SIZE;
//     }
//     jug_y--;
//   }
//
//   empezar_x = empezar_y = 0;
//
//   for (y = (jug_y - 12); y < (jug_y + 12); y++) {
//     for (x = (jug_x - 16); x < (jug_x + 16); x++) {
//       if (x >= 0 && y >= 0 && x < m->width && y < m->height) {
//         if (!layer)
//           draw(m->mapa[(x + (y * m->width))],
// empezar_x, empezar_y, p.h_offset,
//                p.v_offset, true);
//         else
//           draw(m->elementos[(x + (y * m->width))], empezar_x, empezar_y,
//                p.h_offset, p.v_offset, true, true);
//       }
//       empezar_x += TILE_SIZE;
//     }
//     empezar_x = 0;
//     empezar_y += TILE_SIZE;
//   }
// }

void graphics::renderBackground() {
  SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
}

void graphics::draw(ui_manager *i) {
  SDL_Rect dst, src;

  int containerIndex, lb, ib, bon, img;
  for (containerIndex = 0; i->getContainer(containerIndex) != nullptr;
       containerIndex++) {
    if (i->getContainer(containerIndex)->visible) {
      // Globales para todo el contenedor
      src.w = 1;
      src.h = 26;
      src.y = 0;

      // Fondo del contenedor
      dst.x = i->getContainer(containerIndex)->x;
      dst.y = i->getContainer(containerIndex)->y;
      dst.w = i->getContainer(containerIndex)->width;
      dst.h = i->getContainer(containerIndex)->height;

      SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
                             backgroundColor.b, SDL_ALPHA_OPAQUE);
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
      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->width - 15;
      dst.y = i->getContainer(containerIndex)->y - 12;
      SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

      src.x += 26;
      dst.x -= i->getContainer(containerIndex)->width - 4;
      SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

      src.x += 26;
      dst.y += i->getContainer(containerIndex)->height;
      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->width - 15;
      SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

      src.x += 26;
      dst.x -= i->getContainer(containerIndex)->width - 4;
      SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
    }

    // Labels
    for (lb = 0; i->getContainer(containerIndex)->get_label(lb) != nullptr;
         lb++) {
      if (i->getContainer(containerIndex)->get_label(lb)->get_size() !=
          this->fontSize) {
        this->openFont(
            i->getContainer(containerIndex)->get_label(lb)->get_size());
      }

      SDL_Surface *textSurface = TTF_RenderText_Solid(
          font, i->getContainer(containerIndex)->get_label(lb)->get_text(),
          fontColor);
      SDL_Texture *textTexture =
          SDL_CreateTextureFromSurface(renderer, textSurface);

      if (textSurface != nullptr) {
        dst.x = i->getContainer(containerIndex)->get_label(lb)->x +
                i->getContainer(containerIndex)->x;
        dst.y = i->getContainer(containerIndex)->get_label(lb)->y +
                i->getContainer(containerIndex)->y;
        dst.w = textSurface->w;
        dst.h = textSurface->h;

        SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
      }
    }

    // InputBoxes
    for (ib = 0; i->getContainer(containerIndex)->get_input_box(ib) != nullptr;
         ++ib) {
      // Se dibuja el input box
      src.x = (i->getContainer(containerIndex)->get_input_box(ib)->focused &&
               i->getContainer(containerIndex)->focused && !i->blocked)
                  ? 1
                  : 3;
      src.y = 26;
      src.w = 1;
      src.h = 14;

      dst.x = i->getContainer(containerIndex)->get_input_box(ib)->x +
              i->getContainer(containerIndex)->x;
      dst.y = i->getContainer(containerIndex)->get_input_box(ib)->y +
              i->getContainer(containerIndex)->y;
      dst.w = 1;
      dst.h = 14;

      SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
      dst.x++;
      src.x--;
      for (int an = 0;
           an < i->getContainer(containerIndex)->get_input_box(ib)->width - 2;
           an++) {
        SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);
        dst.x++;
      }
      src.x++;
      SDL_RenderCopy(renderer, uiImagesTexture, &src, &dst);

      // La etiqueta del inputbox
      if (8 != this->fontSize) {
        this->openFont(8);
      }

      SDL_Surface *titleSurface = TTF_RenderText_Solid(
          font, i->getContainer(containerIndex)->get_input_box(ib)->get_title(),
          fontColor);
      SDL_Texture *titleTexture =
          SDL_CreateTextureFromSurface(renderer, titleSurface);

      dst.x = i->getContainer(containerIndex)->get_input_box(ib)->x +
              i->getContainer(containerIndex)->x - titleSurface->w - 5;
      dst.y += 3;
      dst.w = titleSurface->w;
      dst.h = titleSurface->h;

      SDL_RenderCopy(renderer, titleTexture, nullptr, &dst);

      if (i->getContainer(containerIndex)
              ->get_input_box(ib)
              ->current_length() != 0) {
        SDL_Surface *textSurface = TTF_RenderText_Solid(
            font,
            i->getContainer(containerIndex)->get_input_box(ib)->get_text(),
            fontColor);
        dst.x += titleSurface->w + 10;
        dst.w =
            std::min(i->getContainer(containerIndex)->get_input_box(ib)->width,
                     textSurface->w);

        SDL_Texture *textTexture =
            SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
      }

      SDL_DestroyTexture(titleTexture);
      SDL_FreeSurface(titleSurface);
    }

    // Botones
    for (bon = 0; i->getContainer(containerIndex)->get_button(bon) != nullptr;
         bon++) {
      if (i->getContainer(containerIndex)->get_button(bon)->press == 0)
        SDL_SetRenderDrawColor(renderer, 63, 63, 63, SDL_ALPHA_OPAQUE);
      else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->get_button(bon)->x;
      dst.y = i->getContainer(containerIndex)->y +
              i->getContainer(containerIndex)->get_button(bon)->y;
      dst.w = i->getContainer(containerIndex)->get_button(bon)->width;
      dst.h = i->getContainer(containerIndex)->get_button(bon)->height;

      SDL_RenderFillRect(renderer, &dst);

      if (i->getContainer(containerIndex)->get_button(bon)->press == 0)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      else
        SDL_SetRenderDrawColor(renderer, 63, 63, 63, SDL_ALPHA_OPAQUE);

      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->get_button(bon)->x;
      dst.y = i->getContainer(containerIndex)->y +
              i->getContainer(containerIndex)->get_button(bon)->y;
      dst.w = i->getContainer(containerIndex)->get_button(bon)->width;
      dst.h = 1;

      SDL_RenderFillRect(renderer, &dst);

      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->get_button(bon)->x;
      dst.y = i->getContainer(containerIndex)->y +
              i->getContainer(containerIndex)->get_button(bon)->y +
              i->getContainer(containerIndex)->get_button(bon)->height;
      dst.w = i->getContainer(containerIndex)->get_button(bon)->width;
      dst.h = 1;

      SDL_RenderFillRect(renderer, &dst);

      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->get_button(bon)->x;
      dst.y = i->getContainer(containerIndex)->y +
              i->getContainer(containerIndex)->get_button(bon)->y;
      dst.w = 1;
      dst.h = i->getContainer(containerIndex)->get_button(bon)->height;

      SDL_RenderFillRect(renderer, &dst);

      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->get_button(bon)->x +
              i->getContainer(containerIndex)->get_button(bon)->width;
      dst.y = i->getContainer(containerIndex)->y +
              i->getContainer(containerIndex)->get_button(bon)->y;
      dst.w = 1;
      dst.h = i->getContainer(containerIndex)->get_button(bon)->height;

      SDL_RenderFillRect(renderer, &dst);

      // Dibujar la etiqueta
      if (this->fontSize != 8) this->openFont(8);

      SDL_Surface *titlebSurface = TTF_RenderText_Solid(
          font, i->getContainer(containerIndex)->get_button(bon)->get_title(),
          fontColor);
      dst.y = i->getContainer(containerIndex)->y +
              i->getContainer(containerIndex)->get_button(bon)->y +
              (i->getContainer(containerIndex)->get_button(bon)->height / 2) -
              (titlebSurface->h / 2);
      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->get_button(bon)->x +
              (i->getContainer(containerIndex)->get_button(bon)->width / 2) -
              (titlebSurface->w / 2);
      dst.w = titlebSurface->w;
      dst.h = titlebSurface->h;

      SDL_Texture *titlebTexture =
          SDL_CreateTextureFromSurface(renderer, titlebSurface);
      SDL_RenderCopy(renderer, titlebTexture, nullptr, &dst);
      SDL_DestroyTexture(titlebTexture);
      SDL_FreeSurface(titlebSurface);
    }

    // Imagenes
    for (img = 0; i->getContainer(containerIndex)->get_image(img) != nullptr;
         img++) {
      if (i->getContainer(containerIndex)->get_image(img)->img != nullptr) {
        dst.y = i->getContainer(containerIndex)->y +
                i->getContainer(containerIndex)->get_image(img)->y;
        dst.x = i->getContainer(containerIndex)->x +
                i->getContainer(containerIndex)->get_image(img)->x;
        dst.w = i->getContainer(containerIndex)->get_image(img)->img->w;
        dst.h = i->getContainer(containerIndex)->get_image(img)->img->h;
        // TODO(carlos): Fix this images thing
        // SDL_BlitSurface(i->getContainer(containerIndex)->getImagen(img)->img,
        // NULL, bbuffer, &dst);
      } else {
        logger::error("no image to draw");
      }
    }

    // Selectores
    for (int sel = 0;
         i->getContainer(containerIndex)->get_selector(sel) != nullptr; sel++) {
      src.x = 4;
      src.y = 26;
      src.w = 9;
      src.h = 14;

      dst.x = i->getContainer(containerIndex)->x +
              i->getContainer(containerIndex)->get_selector(sel)->x;
      dst.y = i->getContainer(containerIndex)->y +
              i->getContainer(containerIndex)->get_selector(sel)->y;
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
      SDL_Surface *seleccionado =
          TTF_RenderText_Solid(font,
                               i->getContainer(containerIndex)
                                   ->get_selector(sel)
                                   ->get_selected()
                                   .c_str(),
                               fontColor);
      i->getContainer(containerIndex)->get_selector(sel)->width =
          seleccionado->w + 8;

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

      SDL_Texture *seleccionadoTexture =
          SDL_CreateTextureFromSurface(renderer, seleccionado);
      SDL_RenderCopy(renderer, seleccionadoTexture, nullptr, &dst);

      SDL_DestroyTexture(seleccionadoTexture);
      SDL_FreeSurface(seleccionado);
    }
  }
}

// void graphics::draw(player &p) {
//   int tilexinic = p.x - 16;
//   int tileyinic = p.y - 12;
//   if (this->fontSize != 8) this->openFont(8);
//
//   SDL_Rect dst, src;
//   dst.x = 1024 / 2;
//   dst.y = 768 / 2;
//   dst.w = 32;
//   dst.h = 32;
//
//   src.x = 32 * p.spriteState;
//   src.y = 32 * (p.traje - 1);
//   src.w = 32;
//   src.h = 32;
//
//   SDL_RenderCopy(renderer, playersTexture, &src, &dst);
//
//   // Dibuja etiqueta de tu personaje
//   SDL_Surface *nombre =
//       TTF_RenderText_Solid(font, (char *)p.nombre.c_str(), fontColor);
//   SDL_Texture *nombreTexture = SDL_CreateTex
// tureFromSurface(renderer, nombre);
//
//   dst.x = 528 - (nombre->w / 2);
//   dst.y = 420;
//   dst.w = nombre->w;
//   dst.h = nombre->h;
//
//   SDL_RenderCopy(renderer, nombreTexture, nullptr, &dst);
//
//   SDL_DestroyTexture(nombreTexture);
//   SDL_FreeSurface(nombre);
// }

void graphics::flipBuffer() { SDL_RenderPresent(renderer); }

void graphics::openFont(int size) {
  TTF_CloseFont(font);
  font = TTF_OpenFont((basePath + "data/pixel_font.ttf").c_str(), size);
  if (!font) {
    logger::error(TTF_GetError());
  }
  this->fontSize = size;
}
}  // namespace bure
