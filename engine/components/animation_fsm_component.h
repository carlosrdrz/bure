#pragma once

#include "component.h"
#include "sprite_component.h"

namespace bure {
namespace components {

class animation_component : public component {
 using component::component;

 public:
  void addAnimation(std::string id, animation_component a) {
    _animations.emplace(id, a);
  }

  void setAnimation(std::string s) {
    _currentAnimation = s;
  }

 private:
  std::map<std::string, animation_component> _animations;
  std::string _currentAnimation;
};

}  // namespace components
}  // namespace bure
