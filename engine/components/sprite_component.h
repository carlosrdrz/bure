#pragma once

#include <string>

#include "component.h"
#include "graphics.h"

namespace bure {
namespace components {

class sprite_component : public component {
 using component::component;

 public:
  void setAll(sprite_id spriteId, rect src, int width, int height);

  void setSpriteID(sprite_id spriteId);
  void setSrcRect(rect src);
  void setSize(int width, int height);

  int getWidth();
  int getHeight();
  sprite_id getSpriteID();
  rect getSrcRect();

 private:
  int _width, _height;
  sprite_id _spriteId;
  rect _src;
};

}  // namespace components
}  // namespace bure
