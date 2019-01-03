#include "character_entity.h"
#include "components/position_component.h"
#include "components/sprite_component.h"
#include "components/animation_component.h"
#include "../scripts/character_script_component.h"

using namespace bure::components;

void character_entity::init() {
  this->setLayer(1);
  this->addComponent<character_script_component>();

  auto position = this->addComponent<position_component>();
  position->setCoords(496, 368);

  initStateAnimations();
  setStateAnimation(_state);
}

void character_entity::initStateAnimations() {
  std::vector<bure::rect> walkingUpRects;
  walkingUpRects.push_back({ -1, 0, 32, 32 });
  walkingUpRects.push_back({ 63, 0, 32, 32 });

  std::vector<bure::rect> walkingDownRects;
  walkingDownRects.push_back({ 95, 0, 32, 32 });
  walkingDownRects.push_back({ 159, 0, 32, 32 });

  std::vector<bure::rect> walkingLeftRects;
  walkingLeftRects.push_back({ 191, 0, 32, 32 });
  walkingLeftRects.push_back({ 255, 0, 32, 32 });

  std::vector<bure::rect> walkingRightRects;
  walkingRightRects.push_back({ 287, 0, 32, 32 });
  walkingRightRects.push_back({ 351, 0, 32, 32 });

  std::vector<bure::rect> standingUpRects;
  standingUpRects.push_back({ 31, 0, 32, 32 });

  std::vector<bure::rect> standingDownRects;
  standingDownRects.push_back({ 127, 0, 32, 32 });

  std::vector<bure::rect> standingLeftRects;
  standingLeftRects.push_back({ 223, 0, 32, 32 });

  std::vector<bure::rect> standingRightRects;
  standingRightRects.push_back({ 319, 0, 32, 32 });

  _animations_rects.emplace(character_state::walking_up, walkingUpRects);
  _animations_rects.emplace(character_state::walking_down, walkingDownRects);
  _animations_rects.emplace(character_state::walking_left, walkingLeftRects);
  _animations_rects.emplace(character_state::walking_right, walkingRightRects);
  _animations_rects.emplace(character_state::standing_up, standingUpRects);
  _animations_rects.emplace(character_state::standing_down, standingDownRects);
  _animations_rects.emplace(character_state::standing_left, standingLeftRects);
  _animations_rects.emplace(character_state::standing_right, standingRightRects);
}

void character_entity::setStateAnimation(character_state cs) {
  this->removeComponentByType<animation_component>();

  auto animation = this->addComponent<animation_component>();
  animation->setScale(2);

  for (auto& r : _animations_rects[this->getState()]) {
    animation->addSprite("personajes", r, 32, 32);
  }

  animation->setAnimationTicks(5);
}
