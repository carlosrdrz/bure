#pragma once

#include "engine/game_map.h"
#include "entities/entity.h"
#include "game.h"
#include "types.h"
#include "utils/map_generator.h"

class example_game : public bure::game {
 public:
  void init() override;

  bool canWalk(bure::map_coords mc);
  bool anyEntityIn(bure::map_coords mc);
  bure::entities::entity* entityIn(bure::map_coords mc);

  std::unique_ptr<bure::game_map> generateMap();

 private:
  bool _godMode = false;
  std::unique_ptr<map_generator> _mapGenerator;

  void startGame(int unused);
};
