#pragma once

#include "entities/entity.h"
#include "graphics.h"

using namespace bure::entities;

enum class animation_id {
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
  void update() override;

  bool isWalking();
  bool isStanding();

  void setPosition(bure::map_coords mc);
  void setMapPosition(bure::map_coords mc);
  void setVelocity(int v);

 protected:
  virtual void initAnimations();
  void setAnimation(animation_id cs);
  bool animationWalking();
  bool animationStanding();
  void stopWalkingAnimation();

  void moveTo(bure::direction d);
  void randomlyMove();

  animation_id _animationId = animation_id::standing_down;
  std::map<animation_id, std::vector<bure::rect>> _animations_rects;

 private:
  entity* _shadowEntity = nullptr;
};
