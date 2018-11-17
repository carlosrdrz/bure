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

typedef std::string cache_id;
typedef std::string sprite_id;

struct cached_text {
  cache_id id;
  SDL_Texture* texture;
  SDL_Surface* surface;
};

struct cached_sprite {
  cache_id id;
  SDL_Texture* texture;
  SDL_Surface* surface;
};

class graphics2 {
 public:
  explicit graphics2(std::string basePath);
  ~graphics2();

  void drawSprite(sprite_id spriteId, rect s, rect d);
  void drawRect(rect r, color c);
  void drawText(std::string text, int x, int y, int size, color c);
  void drawTextCentered(std::string text, int x, int y, int size, color c);

  void clean();
  void flipBuffer();

 private:
  cached_sprite getCachedSprite(sprite_id id);
  cached_text getCachedText(std::string text, int size, color c);

  void drawFullTexture(SDL_Texture *txt, rect r);
  void setRenderColor(color c);
  SDL_Rect rectToSDLRect(rect r);
  void openFont(int size);

  std::string basePath;
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  int fontSize;
  float scale;

  std::map<cache_id, cached_sprite> _cached_sprites;
  std::map<cache_id, cached_text> _cached_texts;
};
}  // namespace bure
