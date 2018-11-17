#pragma once

#include <string>

#include "component.h"
#include "sprite_manager.h"
#include "graphics2.h"

namespace bure {
namespace components {

class sprite_component : public component {
 using component::component;

 public:
  void setSpriteID(sprite_id spriteId) { _spriteId = spriteId; }
  void setSrcRect(rect src) { _src = src; }
  void setSize(int width, int height) {
    _width = width;
    _height = height;
  }

  int getWidth() { return _width; }
  int getHeight() { return _height; }
  sprite_id getSpriteID() { return _spriteId; }
  rect getSrcRect() { return _src; }

 private:
  int _width, _height;
  sprite_id _spriteId;
  rect _src;
};

}  // namespace components
}  // namespace bure
