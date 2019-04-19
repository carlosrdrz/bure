#pragma once

#include "component.h"
#include "sprite_component.h"

namespace bure {
namespace components {

class animation_component : public component {
 using component::component;

 public:
  void addSprite(sprite_component s);
  void addSprite(sprite_id spriteId, rect src, int width, int height);
  void setAnimationTicks(int t);
  void setScale(float scale);
  void setLooping(bool l);

  sprite_component tickAndGetSprite();
  float getScale();

 private:
  std::vector<sprite_component> _sprites;
  bool _loop = true;
  int _currentSprite = 0;
  int _animationTicks = 10;
  int _currentTicks = 0;
  float _scale = 1.0;
};

}  // namespace components
}  // namespace bure
