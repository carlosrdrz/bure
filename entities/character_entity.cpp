#include "character_entity.h"
#include "../components/movement_component.h"
#include "components/animation_component.h"
#include "components/position_component.h"
#include "components/solid_component.h"
#include "components/sprite_component.h"
#include "engine.h"

#include <random>

using namespace bure::components;

void character_entity::init() {
  setLayer(1);

  bure::map_coords mc = {0, 0};
  auto map = bure::engine::get().getMap();
  this->addComponent<solid_component>();
  auto world_pos = map->mapToWorld(mc);
  auto pos = this->addComponent<position_component>();
  pos->setPosition(world_pos);
  auto m = this->addComponent<movement_component>();
  m->setState(movement_state::standing_down);
  m->setVelocity(2);
  m->setPosition(mc);

  initAnimations();
  setAnimation(animation_id::standing_down);
}

void character_entity::update() {
  if (isStanding() && animationWalking()) {
    stopWalkingAnimation();
  }
}

void character_entity::setPosition(bure::map_coords mc) {
  auto map = bure::engine::get().getMap();
  auto wc = map->mapToWorld(mc);

  auto pos = getComponentByType<position_component>();
  pos->setPosition(wc);
  auto m = getComponentByType<movement_component>();
  m->setPosition(mc);
  auto s = getComponentByType<solid_component>();
  s->setPosition(mc);
}

void character_entity::setVelocity(int v) {
  auto m = getComponentByType<movement_component>();
  m->setVelocity(v);
}

bool character_entity::isWalking() { return !isStanding(); }

bool character_entity::isStanding() {
  auto m = getComponentByType<movement_component>();
  auto state = m->getState();

  return state == movement_state::standing_up ||
         state == movement_state::standing_down ||
         state == movement_state::standing_left ||
         state == movement_state::standing_right;
}

void character_entity::moveTo(direction d) {
  auto map = bure::engine::get().getMap();
  auto m = getComponentByType<movement_component>();
  movement_state nextState = m->getState();
  bure::map_coords currentPosition = m->getPosition();
  bure::map_coords nextPosition = currentPosition;
  animation_id nextAnimation;

  switch (d) {
    case direction::left:
      nextState = movement_state::walking_left;
      nextAnimation = animation_id::walking_left;
      nextPosition.x -= 1;
      break;
    case direction::right:
      nextState = movement_state::walking_right;
      nextAnimation = animation_id::walking_right;
      nextPosition.x += 1;
      break;
    case direction::up:
      nextState = movement_state::walking_up;
      nextAnimation = animation_id::walking_up;
      nextPosition.y -= 1;
      break;
    case direction::down:
      nextState = movement_state::walking_down;
      nextAnimation = animation_id::walking_down;
      nextPosition.y += 1;
      break;
    default:
      break;
  }

  if (map->canWalk(nextPosition)) {
    m->setState(nextState);
    setAnimation(nextAnimation);
  }
}

void character_entity::randomlyMove() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 100);
  int randomNumber = dis(gen);

  switch (randomNumber) {
    case 0:
      moveTo(direction::left);
      break;
    case 1:
      moveTo(direction::right);
      break;
    case 2:
      moveTo(direction::up);
      break;
    case 3:
      moveTo(direction::down);
      break;
    default:
      break;
  }
}

void character_entity::initAnimations() {
  std::vector<bure::rect> walkingUpRects;
  walkingUpRects.push_back({-1, 64, 32, 32});
  walkingUpRects.push_back({63, 64, 32, 32});

  std::vector<bure::rect> walkingDownRects;
  walkingDownRects.push_back({95, 64, 32, 32});
  walkingDownRects.push_back({159, 64, 32, 32});

  std::vector<bure::rect> walkingLeftRects;
  walkingLeftRects.push_back({191, 64, 32, 32});
  walkingLeftRects.push_back({255, 64, 32, 32});

  std::vector<bure::rect> walkingRightRects;
  walkingRightRects.push_back({287, 64, 32, 32});
  walkingRightRects.push_back({351, 64, 32, 32});

  std::vector<bure::rect> standingUpRects;
  standingUpRects.push_back({31, 64, 32, 32});

  std::vector<bure::rect> standingDownRects;
  standingDownRects.push_back({127, 64, 32, 32});

  std::vector<bure::rect> standingLeftRects;
  standingLeftRects.push_back({223, 64, 32, 32});

  std::vector<bure::rect> standingRightRects;
  standingRightRects.push_back({319, 64, 32, 32});

  _animations_rects.emplace(animation_id::walking_up, walkingUpRects);
  _animations_rects.emplace(animation_id::walking_down, walkingDownRects);
  _animations_rects.emplace(animation_id::walking_left, walkingLeftRects);
  _animations_rects.emplace(animation_id::walking_right, walkingRightRects);
  _animations_rects.emplace(animation_id::standing_up, standingUpRects);
  _animations_rects.emplace(animation_id::standing_down, standingDownRects);
  _animations_rects.emplace(animation_id::standing_left, standingLeftRects);
  _animations_rects.emplace(animation_id::standing_right, standingRightRects);
}

// TODO(carlosrdrz): We don't want to recreate all animations everytime here.
// It would be better if we could save them somewhere and reuse.
void character_entity::setAnimation(animation_id animationId) {
  _animationId = animationId;
  this->removeComponentByType<animation_component>();

  auto animation = this->addComponent<animation_component>();
  animation->setScale(2);

  for (auto& r : _animations_rects[animationId]) {
    animation->addSprite("personajes.png", r, 32, 32);
  }

  animation->setAnimationTicks(10);
}

bool character_entity::animationWalking() {
  return (_animationId == animation_id::walking_up ||
          _animationId == animation_id::walking_down ||
          _animationId == animation_id::walking_left ||
          _animationId == animation_id::walking_right);
}

bool character_entity::animationStanding() { return !animationWalking(); }

void character_entity::stopWalkingAnimation() {
  switch (_animationId) {
    case animation_id::walking_up:
      setAnimation(animation_id::standing_up);
      break;
    case animation_id::walking_down:
      setAnimation(animation_id::standing_down);
      break;
    case animation_id::walking_left:
      setAnimation(animation_id::standing_left);
      break;
    case animation_id::walking_right:
      setAnimation(animation_id::standing_right);
      break;
    default:
      break;
  }
}
