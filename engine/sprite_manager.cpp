#include "sprite_manager.h"
#include "utils/logger.h"
#include "utils/config.h"

#include "SDL_image.h"

namespace bure {

SDL_Surface* sprite_manager::getSprite(sprite_id id) {
  auto search = _sprites.find(id);
  if (search != _sprites.end()) {
    return search->second.get();
  }

  logger::debug("loading sprite %s", (_basePath + "data/" + id + ".png").c_str());
  auto surface = IMG_Load((_basePath + "data/" + id + ".png").c_str());
  if (!surface) {
    logger::error("IMG_Load: %s", IMG_GetError());
    // TODO(carlosrdrz): deal with this error here. maybe just crash?
  }

  std::unique_ptr<SDL_Surface> surfacePtr(surface);
  _sprites[id] = std::move(surfacePtr);
  return _sprites[id].get();
}

}  // namespace bure
