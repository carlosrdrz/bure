#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "../ui/ui_manager.h"
#include "entities/entity.h"
#include "event_manager.h"
#include "systems/system.h"

namespace bure {

class engine {
 public:
  engine();

  void addSystem(std::unique_ptr<systems::system> s);
  void addEntity(std::unique_ptr<entities::entity> e);

  std::vector<std::reference_wrapper<entities::entity>> getEntities();

  void update();

  static engine& get() {
    if (_instance == nullptr) {
      _instance = new engine();
    }

    return *_instance;
  }
 private:
  static engine* _instance;
  std::unique_ptr<bure::ui::ui_manager> _uiManager;

  std::vector<std::unique_ptr<bure::systems::system>> _systems;
  std::vector<std::unique_ptr<bure::entities::entity>> _entities;
};

}  // namespace bure
