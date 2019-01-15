#pragma once

#include "character_entity.h"

class enemy_entity : public character_entity {
 using character_entity::character_entity;

 public:
  void init() override;
  void initStateAnimations() override;
};
