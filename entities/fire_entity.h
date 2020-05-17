#pragma once

#include "entities/entity.h"
#include "types.h"

using namespace bure::entities;

class fire_entity : public entity {
  using entity::entity;

 public:
  void init() override;
  void setPosition(bure::map_coords mc);
};
