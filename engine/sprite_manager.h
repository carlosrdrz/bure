#pragma once

#include <string>
#include <memory>
#include <map>

#include "SDL_image.h"

namespace bure {

typedef std::string sprite_id;

class sprite_manager {
 public:
  sprite_manager(std::string basePath) : _basePath(basePath) {}

  SDL_Surface* getSprite(sprite_id id);

 private:
  std::string _basePath;
  std::map<sprite_id, std::unique_ptr<SDL_Surface>> _sprites;
};

}  // namespace bure
