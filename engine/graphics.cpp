#include <algorithm>

#include "graphics.h"
#include "utils/config.h"
#include "utils/logger.h"

#define TILE_SIZE 32

namespace bure {
// todo: fix basepath
graphics::graphics(std::string basePath, int width, int height) :
  basePath(basePath) {
  atexit(SDL_Quit);

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    logger::error(SDL_GetError());
  }

  // Init video
  scale = config::instance.getFloatValueOf("scale", 1.0);
  auto resX = width * scale;
  auto resY = height * scale;

  Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI & SDL_WINDOW_OPENGL;
  if (config::instance.getBoolValueOf("fullscreen")) flags |= SDL_WINDOW_FULLSCREEN;
  window = SDL_CreateWindow("Bure", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, resX, resY, flags);
  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_RenderSetScale(renderer, scale, scale);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Init SDL_ttf and load font
  if (TTF_Init() < 0) logger::error(SDL_GetError());

  fontSize = 16;
  font = TTF_OpenFont((basePath + "resources/pixel_font.ttf").c_str(), fontSize);
  if (!font) logger::error(TTF_GetError());
}

graphics::~graphics() {
  TTF_CloseFont(font);
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void graphics::clean() {
  setRenderColor({ 0, 0, 0, 255 });
  SDL_RenderClear(renderer);
}

SDL_Rect graphics::rectToSDLRect(rect r) {
  SDL_Rect rect = {r.x, r.y, r.width, r.height};
  return rect;
}

void graphics::setRenderColor(color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.alpha);
}

void graphics::drawFullTexture(SDL_Texture *txt, rect r) {
  SDL_Rect dst = rectToSDLRect(r);
  SDL_RenderCopy(renderer, txt, nullptr, &dst);
}

void graphics::drawRect(rect r, color c) {
  SDL_Rect dst = rectToSDLRect(r);
  setRenderColor(c);
  SDL_RenderFillRect(renderer, &dst);
}

void graphics::drawSprite(sprite_id spriteId, rect s, rect d) {
  SDL_Rect src = rectToSDLRect(s);
  SDL_Rect dst = rectToSDLRect(d);
  auto cachedSprite = getCachedSprite(spriteId);
  SDL_RenderCopy(renderer, cachedSprite.texture, &src, &dst);
}

void graphics::drawText(std::string text, int x, int y, int size, color c) {
  auto cachedText = getCachedText(text, size, c);
  auto textSurface = cachedText.surface;
  drawFullTexture(cachedText.texture, {x, y, textSurface->w, textSurface->h});
}

void graphics::drawTextCentered(std::string text, int x, int y, int size,
                                 color c) {
  auto cachedText = getCachedText(text, size, c);
  auto textSurface = cachedText.surface;
  int xCenter = x - textSurface->w / 2;
  int yCenter = y - textSurface->h / 2;
  drawFullTexture(cachedText.texture,
                  { xCenter, yCenter, textSurface->w, textSurface->h });
}

void graphics::flipBuffer() { SDL_RenderPresent(renderer); }

void graphics::openFont(int size) {
  TTF_CloseFont(font);
  font = TTF_OpenFont((basePath + "resources/pixel_font.ttf").c_str(), size);
  if (!font) {
    logger::error(TTF_GetError());
  }
  this->fontSize = size;
}

cached_sprite graphics::getCachedSprite(sprite_id id) {
  auto search = _cached_sprites.find(id);
  if (search != _cached_sprites.end()) {
    return search->second;
  }

  // TODO(carlosrdrz): we are not removing this memory here
  auto sprite = IMG_Load((basePath + "resources/sprites/" + id).c_str());
  if (!sprite) {
    logger::error("IMG_Load: %s", IMG_GetError());
    // TODO(carlosrdrz): deal with this error here. maybe just crash?
  }

  auto texture = SDL_CreateTextureFromSurface(renderer, sprite);

  struct cached_sprite entry = {id, texture, sprite};
  _cached_sprites[id] = entry;
  return _cached_sprites[id];
}

cached_text graphics::getCachedText(std::string text, int size, color c) {
  auto search = _cached_texts.find(text);
  if (search != _cached_texts.end()) {
    return search->second;
  }

  // TODO(carlosrdrz): we are not removing this memory here
  if (size != this->fontSize) this->openFont(size);
  auto surface = TTF_RenderText_Solid(font, text.c_str(), {(Uint8)c.r, (Uint8)c.g, (Uint8)c.b});
  auto texture = SDL_CreateTextureFromSurface(renderer, surface);

  struct cached_text entry = { text, texture, surface };
  _cached_texts[text] = entry;
  return _cached_texts[text];
}

}  // namespace bure
