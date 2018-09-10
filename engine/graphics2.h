#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <list>
#include <string>

#include "game_map.h"
#include "ui/ui_manager.h"
#include "utils/config.h"

namespace bure {

struct color {
  int r;
  int g;
  int b;
  int alpha;
};

struct rect {
  int x;
  int y;
  int width;
  int height;
};

class graphics2 {
 public:
  explicit graphics2(std::string basePath);
  ~graphics2();

  void drawSprite();
  void drawRect(rect r, color c);
  void drawText(std::string text, int x, int y, int size, color c);
  void drawTextCentered(std::string text, int x, int y, int size, color c);

  void clean();
  void flipBuffer();

 private:
  void drawFullTexture(SDL_Texture *txt, rect r);
  void setRenderColor(color c);
  SDL_Rect rectToSDLRect(rect r);
  void openFont(int size);

  SDL_Window *window;
  SDL_Renderer *renderer;
  std::string basePath;
  TTF_Font *font;
  int fontSize;
  float scale;
};
}  // namespace bure
