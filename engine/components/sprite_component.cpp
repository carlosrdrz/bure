#include "sprite_component.h"

namespace bure {
namespace components {

void sprite_component::setAll(sprite_id spriteId, rect src, int width,
                              int height) {
  _spriteId = spriteId;
  _src = src;
  _width = width;
  _height = height;
}

void sprite_component::setSpriteID(sprite_id spriteId) { _spriteId = spriteId; }
void sprite_component::setSrcRect(rect src) { _src = src; }
void sprite_component::setSize(int width, int height) {
  _width = width;
  _height = height;
}
void sprite_component::setScale(int scale) { _scale = scale; }

int sprite_component::getWidth() { return _width; }
int sprite_component::getHeight() { return _height; }
sprite_id sprite_component::getSpriteID() { return _spriteId; }
rect sprite_component::getSrcRect() { return _src; }
int sprite_component::getScale() { return _scale; }

}  // namespace components
}  // namespace bure
