#include "animation_component.h"

namespace bure {
namespace components {

void animation_component::addSprite(sprite_component s) {
  _sprites.push_back(s);
}

void animation_component::addSprite(sprite_id spriteId, rect src, int width,
                                    int height) {
  auto sprite = sprite_component(_entity);
  sprite.setAll(spriteId, src, width, height);
  addSprite(sprite);
}

void animation_component::setAnimationTicks(int t) { _animationTicks = t; }

sprite_component animation_component::tickAndGetSprite() {
  auto s = _sprites.at(_currentSprite);

  if (++_currentTicks > _animationTicks) {
    _currentSprite = ++_currentSprite % _sprites.size();
    _currentTicks = 0;
  }

  return s;
}

}  // namespace components
}  // namespace bure
