#include "enemy_entity.h"
#include "engine.h"
#include "components/position_component.h"
#include "components/sprite_component.h"
#include "components/animation_component.h"

using namespace bure::components;

void enemy_entity::init() {
  this->setLayer(1);

  auto map = bure::engine::get().getMap();
  auto screen_pos = map->mapToScreen({ 67, 42 });
  auto position = this->addComponent<position_component>();
  position->setCoords(screen_pos.x, screen_pos.y);

  initStateAnimations();
  setStateAnimation(_state);
}

void enemy_entity::initStateAnimations() {
  std::vector<bure::rect> walkingUpRects;
  walkingUpRects.push_back({ -1, 128, 32, 32 });
  walkingUpRects.push_back({ 63, 128, 32, 32 });

  std::vector<bure::rect> walkingDownRects;
  walkingDownRects.push_back({ 95, 128, 32, 32 });
  walkingDownRects.push_back({ 159, 128, 32, 32 });

  std::vector<bure::rect> walkingLeftRects;
  walkingLeftRects.push_back({ 191, 128, 32, 32 });
  walkingLeftRects.push_back({ 255, 128, 32, 32 });

  std::vector<bure::rect> walkingRightRects;
  walkingRightRects.push_back({ 287, 128, 32, 32 });
  walkingRightRects.push_back({ 351, 128, 32, 32 });

  std::vector<bure::rect> standingUpRects;
  standingUpRects.push_back({ 31, 128, 32, 32 });

  std::vector<bure::rect> standingDownRects;
  standingDownRects.push_back({ 127, 128, 32, 32 });

  std::vector<bure::rect> standingLeftRects;
  standingLeftRects.push_back({ 223, 128, 32, 32 });

  std::vector<bure::rect> standingRightRects;
  standingRightRects.push_back({ 319, 128, 32, 32 });

  _animations_rects.emplace(character_state::walking_up, walkingUpRects);
  _animations_rects.emplace(character_state::walking_down, walkingDownRects);
  _animations_rects.emplace(character_state::walking_left, walkingLeftRects);
  _animations_rects.emplace(character_state::walking_right, walkingRightRects);
  _animations_rects.emplace(character_state::standing_up, standingUpRects);
  _animations_rects.emplace(character_state::standing_down, standingDownRects);
  _animations_rects.emplace(character_state::standing_left, standingLeftRects);
  _animations_rects.emplace(character_state::standing_right, standingRightRects);
}
