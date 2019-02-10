#include "enemy_entity.h"
#include "components/map_position_component.h"
#include "../components/stats_component.h"
#include "engine.h"
#include "utils/pathfinding.h"

using namespace bure::components;

void enemy_entity::init() {
  character_entity::init();

  setPosition({67, 42});
  setVelocity(1);
}

void enemy_entity::follow(character_entity *c) { _character = c; }

void enemy_entity::update() {
  character_entity::update();

  if (isStanding() && _character != nullptr) {
    auto dst_mc = _character->getComponentByType<map_position_component>();
    auto src_mc = getComponentByType<map_position_component>();
    auto dst_pos = dst_mc->getPosition();
    auto src_pos = src_mc->getPosition();
    auto path = bure::pathfinding::a_star(src_pos, dst_pos);

    if (!path.empty()) {
      auto next_pos = path.front();
      auto direction = getDirectionFromCoords(src_pos, next_pos);
      moveTo(direction);
    }
  }
}

void enemy_entity::initAnimations() {
  std::vector<bure::rect> walkingUpRects;
  walkingUpRects.push_back({-1, 128, 32, 32});
  walkingUpRects.push_back({63, 128, 32, 32});

  std::vector<bure::rect> walkingDownRects;
  walkingDownRects.push_back({95, 128, 32, 32});
  walkingDownRects.push_back({159, 128, 32, 32});

  std::vector<bure::rect> walkingLeftRects;
  walkingLeftRects.push_back({191, 128, 32, 32});
  walkingLeftRects.push_back({255, 128, 32, 32});

  std::vector<bure::rect> walkingRightRects;
  walkingRightRects.push_back({287, 128, 32, 32});
  walkingRightRects.push_back({351, 128, 32, 32});

  std::vector<bure::rect> standingUpRects;
  standingUpRects.push_back({31, 128, 32, 32});

  std::vector<bure::rect> standingDownRects;
  standingDownRects.push_back({127, 128, 32, 32});

  std::vector<bure::rect> standingLeftRects;
  standingLeftRects.push_back({223, 128, 32, 32});

  std::vector<bure::rect> standingRightRects;
  standingRightRects.push_back({319, 128, 32, 32});

  _animations_rects.emplace(animation_id::walking_up, walkingUpRects);
  _animations_rects.emplace(animation_id::walking_down, walkingDownRects);
  _animations_rects.emplace(animation_id::walking_left, walkingLeftRects);
  _animations_rects.emplace(animation_id::walking_right, walkingRightRects);
  _animations_rects.emplace(animation_id::standing_up, standingUpRects);
  _animations_rects.emplace(animation_id::standing_down, standingDownRects);
  _animations_rects.emplace(animation_id::standing_left, standingLeftRects);
  _animations_rects.emplace(animation_id::standing_right, standingRightRects);
}

bure::direction enemy_entity::getDirectionFromCoords(bure::map_coords origin,
                                                     bure::map_coords dst) {
  if (dst == bure::map_coords{origin.x + 1, origin.y}) {
    return bure::direction::right;
  } else if (dst == bure::map_coords{origin.x - 1, origin.y}) {
    return bure::direction::left;
  } else if (dst == bure::map_coords{origin.x, origin.y - 1}) {
    return bure::direction::up;
  } else if (dst == bure::map_coords{origin.x, origin.y + 1}) {
    return bure::direction::down;
  } else {
    return bure::direction::none;
  }
}
