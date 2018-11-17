#include <algorithm>

#include "sprite_manager.h"
#include "graphics2.h"
#include "utils/config.h"
#include "utils/logger.h"

#define TILE_SIZE 32

namespace bure {
// todo: fix basepath
graphics2::graphics2(std::string basePath) :
  basePath(basePath),
  _spriteManager(basePath) {
  atexit(SDL_Quit);

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    logger::error(SDL_GetError());
  }

  // Init video at 1024x768 or scaled
  scale = config::instance.getFloatValueOf("scale", 1.0);
  auto resX = 1024 * scale;
  auto resY = 768 * scale;

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
  font = TTF_OpenFont((basePath + "data/pixel_font.ttf").c_str(), fontSize);
  if (!font) logger::error(TTF_GetError());
}

graphics2::~graphics2() {
  TTF_CloseFont(font);
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void graphics2::clean() { SDL_RenderClear(renderer); }

SDL_Rect graphics2::rectToSDLRect(rect r) {
  SDL_Rect rect = {r.x, r.y, r.width, r.height};
  return rect;
}

void graphics2::setRenderColor(color c) {
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.alpha);
}

void graphics2::drawFullTexture(SDL_Texture *txt, rect r) {
  SDL_Rect dst = rectToSDLRect(r);
  SDL_RenderCopy(renderer, txt, nullptr, &dst);
}

void graphics2::drawRect(rect r, color c) {
  SDL_Rect dst = rectToSDLRect(r);
  setRenderColor(c);
  SDL_RenderFillRect(renderer, &dst);
}

void graphics2::drawSprite(sprite_id spriteId, rect s, rect d) {
  SDL_Rect src = rectToSDLRect(s);
  SDL_Rect dst = rectToSDLRect(d);
  auto texture = getSpriteTexture(spriteId);
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

void graphics2::drawText(std::string text, int x, int y, int size, color c) {
  auto textTexture = getTextTexture(text, size, c);
  auto textSurface = getTextSurface(text);
  if (textTexture != nullptr) {
    drawFullTexture(textTexture, {x, y, textSurface->w, textSurface->h});
  }
}

void graphics2::drawTextCentered(std::string text, int x, int y, int size,
                                 color c) {
  auto textTexture = getTextTexture(text, size, c);
  auto textSurface = getTextSurface(text);
  int xCenter = x - textSurface->w / 2;
  int yCenter = y - textSurface->h / 2;
  if (textSurface != nullptr) {
    drawFullTexture(textTexture,
                    { xCenter, yCenter, textSurface->w, textSurface->h });
  }
}

void graphics2::flipBuffer() { SDL_RenderPresent(renderer); }

void graphics2::openFont(int size) {
  TTF_CloseFont(font);
  font = TTF_OpenFont((basePath + "data/pixel_font.ttf").c_str(), size);
  if (!font) {
    logger::error(TTF_GetError());
  }
  this->fontSize = size;
}

SDL_Texture* graphics2::getSpriteTexture(sprite_id id) {
  auto search = _textures.find(id);
  if (search != _textures.end()) {
    return search->second;
  }

  // TODO(carlosrdrz): we are not removing this memory here
  auto sprite = _spriteManager.getSprite(id);
  auto texture = SDL_CreateTextureFromSurface(renderer, sprite);
  _textures[id] = texture;
  return _textures[id];
}

SDL_Texture* graphics2::getTextTexture(std::string text, int size, color c) {
  auto search = _textures.find(text);
  if (search != _textures.end()) {
    return search->second;
  }

  // TODO(carlosrdrz): we are not removing this memory here
  if (size != this->fontSize) this->openFont(size);

  auto textSurface = TTF_RenderText_Solid(font, text.c_str(), {(Uint8)c.r, (Uint8)c.g, (Uint8)c.b});
  auto texture = SDL_CreateTextureFromSurface(renderer, textSurface);
  _textSurfaces[text] = textSurface;
  _textures[text] = texture;
  return _textures[text];
}

SDL_Surface* graphics2::getTextSurface(std::string text) {
  return _textSurfaces[text];
}

}  // namespace bure
