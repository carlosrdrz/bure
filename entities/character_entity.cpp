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

  auto animation = this->addComponent<animation_component>();
  animation->addSprite("personajes", { 95, 0, 32, 32 }, 32, 32);
  animation->addSprite("personajes", { 159, 0, 32, 32 }, 32, 32);
  animation->setAnimationTicks(20);
}
