#include "drawing_system.h"

namespace bure {
namespace systems {

drawing_system::drawing_system(std::unique_ptr<bure::graphics2> gr) {
  _graphics = std::move(gr);
}

void drawing_system::init() {
}

void drawing_system::update() {
  _graphics->clean();

  // TODO(carlosrdrz): This sucks :) Need to improve the way we handle layers

  for (auto& r : _renderers) {
    r->render(0);
  }

  for (auto& r : _renderers) {
    r->render(1);
  }

  _graphics->flipBuffer();
}

void drawing_system::addRenderer(std::unique_ptr<bure::renderer> rend) {
  rend->init(_graphics);
  _renderers.emplace_back(std::move(rend));
}

}  // namespace systems
}  // namespace bure
