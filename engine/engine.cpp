#include "engine.h"

namespace bure {
engine::engine() {
    _eventManager = std::make_unique<event_manager>();
    _uiManager = std::make_unique<ui_manager>();
}

void engine::addSystem(system s) {
    s.init();
    systems.emplace(systems.size(), s);
}

void engine::addEntity(entity e) {
    e.init();
    entities.emplace(entities.size(), e);
}
}  // namespace bure
