#pragma once

#include "character_entity.h"

class enemy_entity : public character_entity {
  using character_entity::character_entity;

 public:
  void init() override;
  void update() override;

  void follow(character_entity* c);
  void initAnimations() override;

 private:
  character_entity* _character = nullptr;

  direction getDirectionFromCoords(bure::map_coords origin,
                                   bure::map_coords dst);
};
