#pragma once

#include "entities/entity.h"
#include "graphics.h"

using namespace bure::entities;

enum class character_state {
  walking_up,
  walking_down,
  walking_right,
  walking_left,
  standing_up,
  standing_down,
  standing_right,
  standing_left
};

class character_entity : public entity {
 using entity::entity;

 public:
  void init() override;

  void setState(character_state cs);
  character_state getState();

 protected:
  virtual void initStateAnimations();
  void setStateAnimation(character_state cs);

  character_state _state = character_state::standing_down;
  std::map<character_state, std::vector<bure::rect>> _animations_rects;
};
