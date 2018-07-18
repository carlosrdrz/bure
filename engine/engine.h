#pragma once

#include <memory>
#include <unordered_map>

#include "system.h"
#include "entity.h"
#include "event_manager.h"
#include "ui_manager.h"

namespace bure {
class engine {
 public:
  engine();

  void addSystem(system s);
  void addEntity(entity e);

  event_manager& getEventManager() { return *_eventManager; }
  ui_manager& getUIManager() { return *_uiManager; }

 private:
  std::unique_ptr<event_manager> _eventManager;
  std::unique_ptr<ui_manager> _uiManager;

  std::unordered_map<system_id, system> systems;
  std::unordered_map<entity_id, entity> entities;
};
}  // namespace bure
