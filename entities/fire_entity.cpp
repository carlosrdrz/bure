#include "fire_entity.h"

#include "components/animation_component.h"
#include "components/position_component.h"
#include "engine.h"

using namespace bure::components;

void fire_entity::init() {
  setLayer(1);

  auto positionComponent = this->addComponent<position_component>();
  positionComponent->setPosition({0, 0});

  auto animation = this->addComponent<animation_component>();
  animation->setScale(2);
  animation->setLooping(false);

  animation->addSprite("fire.png", {0, 32, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {16, 32, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {32, 32, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {48, 32, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {0, 48, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {16, 48, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {32, 48, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {48, 48, 16, 16}, 32, 32);
  animation->addSprite("fire.png", {64, 64, 16, 16}, 32, 32);

  animation->setAnimationTicks(10);
}

void fire_entity::setPosition(bure::map_coords mc) {
  auto map = bure::engine::get().getMap();
  auto p = this->addComponent<position_component>();
  p->setPosition(map->mapToWorld(mc));
}
