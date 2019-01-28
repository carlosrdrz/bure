#pragma once

#include "../components/movement_component.h"
#include "components/position_component.h"
#include "components/solid_component.h"
#include "entities/entity.h"
#include "systems/system.h"

using namespace bure::entities;
using namespace bure::components;

class movement_system : public bure::systems::system {
 public:
  void update() override;

 private:
  void moveEntity(entity &e, position_component &p, movement_component &m,
                  solid_component &s);
  void setPosition(entity &e, position_component &p, movement_component &m,
                   solid_component &s, bure::map_coords pos);
  void stopMoving(movement_component &m);
};
