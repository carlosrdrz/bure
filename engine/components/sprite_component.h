#pragma once

#include <string>

#include "component.h"

namespace bure {
namespace components {

typedef std::string texture_id;

class sprite_component : public component {
 public:
  explicit sprite_component(const bure::entities::entity& e) : component(e) {}

  int getWidth() { return w; }
  int getHeight() { return h; }
  texture_id getTexture() { return texture; }
  int w, h;
  texture_id texture;
};

}  // namespace components
}  // namespace bure
