#pragma once

#include "component.h"
#include "sprite_component.h"

namespace bure {
namespace components {

class animation_component : public component {
 using component::component;

 public:
  void addSprite(sprite_component s) { _sprites.push_back(s); }
  void setAnimationTicks(int t) { _animationTicks = t; }
  sprite_component tickAndGetSprite() {
    auto s = _sprites.at(_currentSprite);

    if (++_currentTicks > _animationTicks) {
      _currentSprite = ++_currentSprite % _sprites.size();
      _currentTicks = 0;
    }

    return s;
  }

 private:
  std::vector<sprite_component> _sprites;
  int _currentSprite = 0;
  int _animationTicks = 10;
  int _currentTicks = 0;
};

}  // namespace components
}  // namespace bure
