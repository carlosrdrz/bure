#pragma once

#include "character_entity.h"

class player_entity : public character_entity {
 using character_entity::character_entity;

 public:
  void init() override;
  void update() override;

   void updateCamera();
 private:
   int _skillCooldown = 50;
   int _skillCooldownCounter = 0;

   void processKeys();
   void castSkills();
};
