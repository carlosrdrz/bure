#include "engine.h"

namespace bure {

engine* engine::_instance;

void engine::addSystem(std::unique_ptr<systems::system> s) {
    s->init();
    _systems.push_back(std::move(s));
}

void engine::addEntity(std::unique_ptr<entities::entity> e) {
    e->init();
    _entities.push_back(std::move(e));
}

void engine::update() {
  for (auto&& s : _systems) {
    s->update();
  }
}

std::vector<std::reference_wrapper<entities::entity>> engine::getEntities() {
  std::vector<std::reference_wrapper<entities::entity>> vector;
  for (auto&& entity : _entities) {
    vector.push_back(std::ref(*entity));
  }
  return vector;
}

}  // namespace bure
