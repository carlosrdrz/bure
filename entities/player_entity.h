#pragma once

#include "character_entity.h"

class player_entity : public character_entity {
 using character_entity::character_entity;

 public:
  void init() override;
  void update() override;

 private:
   int _skillCooldown = 50;
   int _skillCooldownCounter = 0;

   void updateCamera();
   void processKeys();
   void castSkills();
};
