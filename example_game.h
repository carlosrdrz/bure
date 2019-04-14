#pragma once

#include "game.h"
#include "entities/entity.h"
#include "types.h"

class example_game : public bure::game {
 public:
  void init() override;

  bool canWalk(bure::map_coords mc);
  bool anyEntityIn(bure::map_coords mc);
  bure::entities::entity* entityIn(bure::map_coords mc);

 private:
   void startGame(int unused);
};
