#include "character_entity.h"
#include "engine.h"
#include "components/position_component.h"
#include "components/sprite_component.h"
#include "components/animation_component.h"
#include "../scripts/character_script_component.h"

using namespace bure::components;

void character_entity::init() {
  this->setLayer(1);

  auto map = bure::engine::get().getMap();
  auto screen_pos = map->mapToScreen({ 66, 37 });
  auto position = this->addComponent<position_component>();
  position->setCoords(screen_pos.x, screen_pos.y);

  auto script = this->addComponent<character_script_component>();
  script->updateCamera();

  initStateAnimations();
  setStateAnimation(_state);
}

void character_entity::setState(character_state cs) {
  if (cs == _state) return;
  _state = cs;
  setStateAnimation(cs);
}

character_state character_entity::getState() {
  return _state;
}

void character_entity::initStateAnimations() {
  std::vector<bure::rect> walkingUpRects;
  walkingUpRects.push_back({ -1, 64, 32, 32 });
  walkingUpRects.push_back({ 63, 64, 32, 32 });

  std::vector<bure::rect> walkingDownRects;
  walkingDownRects.push_back({ 95, 64, 32, 32 });
  walkingDownRects.push_back({ 159, 64, 32, 32 });

  std::vector<bure::rect> walkingLeftRects;
  walkingLeftRects.push_back({ 191, 64, 32, 32 });
  walkingLeftRects.push_back({ 255, 64, 32, 32 });

  std::vector<bure::rect> walkingRightRects;
  walkingRightRects.push_back({ 287, 64, 32, 32 });
  walkingRightRects.push_back({ 351, 64, 32, 32 });

  std::vector<bure::rect> standingUpRects;
  standingUpRects.push_back({ 31, 64, 32, 32 });

  std::vector<bure::rect> standingDownRects;
  standingDownRects.push_back({ 127, 64, 32, 32 });

  std::vector<bure::rect> standingLeftRects;
  standingLeftRects.push_back({ 223, 64, 32, 32 });

  std::vector<bure::rect> standingRightRects;
  standingRightRects.push_back({ 319, 64, 32, 32 });

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
    animation->addSprite("personajes.png", r, 32, 32);
  }

  animation->setAnimationTicks(10);
}
