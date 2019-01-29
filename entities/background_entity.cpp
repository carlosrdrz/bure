#include "background_entity.h"
#include "components/position_component.h"
#include "components/sprite_component.h"

using namespace bure::components;

void background_entity::init() {
  auto spriteComponent = this->addComponent<sprite_component>();
  auto positionComponent = this->addComponent<position_component>();
  positionComponent->setPosition({0, 0});
  spriteComponent->setSpriteID("background.png");
  spriteComponent->setSrcRect({256, 256, 1024, 768});
  spriteComponent->setSize(1280, 1024);
}
