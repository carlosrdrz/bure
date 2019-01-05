#include "scripts_system.h"

#include "engine.h"
#include "components/script_component.h"

namespace bure {
namespace systems {

void scripts_system::update() {
  for (auto& entity : bure::engine::get().getEntities()) {
    auto& e = entity.get();

    auto script = e.getComponentByType<bure::components::script_component>();
    if (script == nullptr) continue;

    script->onTick();
  }
}

}  // namespace systems
}  // namespace bure
