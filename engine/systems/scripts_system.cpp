#include "scripts_system.h"

#include "engine.h"
#include "components/script_component.h"

namespace bure {
namespace systems {

void scripts_system::update() {
  for (auto& entity : bure::engine::get().getEntities()) {
    auto& e = entity.get();
    e.update();
  }
}

}  // namespace systems
}  // namespace bure
