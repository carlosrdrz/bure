#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <list>
#include <string>

#include "game_map.h"
#include "../ui/ui_manager.h"
#include "utils/config.h"

namespace bure {
class graphics {
 public:
  explicit graphics(std::string basePath);
  ~graphics();

  // void draw(const game &g);
  // void draw(int tile, int x, int y, int h, int v, bool fullLayer, bool layer)
  // todo: no reason these are not const
  // void draw(game_map *m, const player &p, bool layer);
  void draw(bure::ui::ui_manager *i);
  // todo: no reason these are not const
  // void draw(const player &p);

  void renderBackground();

  void openFont(int size);
  void clean();
  void flipBuffer();

 private:
  std::string basePath;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *tilesSurface, *playersSurface, *elementsSurface,
      *uiImagesSurface, *bgSurface;
  SDL_Texture *tilesTexture, *playersTexture, *elementsTexture,
      *uiImagesTexture, *bgTexture;
  SDL_Color backgroundColor, fontColor;
  TTF_Font *font;
  int fontSize;
  float scale;
};
}  // namespace bure
