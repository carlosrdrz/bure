#include "game.h"
#include "engine.h"
#include "utils/logger.h"
#include "entities/entity.h"
#include "components/position_component.h"
#include "components/sprite_component.h"
#include "components/animation_component.h"
#include "scripts/character_script_component.h"

game::game()
    : finished(false) {}

void game::finishGame() {
  finished = true;
}

void game::startMenu() {
  // add entity for background
  auto backgroundEntity = std::make_unique<bure::entities::entity>();
  auto spriteComponent =
      backgroundEntity->addComponent<bure::components::sprite_component>();
  auto positionComponent =
      backgroundEntity->addComponent<bure::components::position_component>();
  positionComponent->setCoords(0, 0);
  spriteComponent->setSpriteID("background");
  spriteComponent->setSrcRect({ 0, 0, 1024, 768 });
  spriteComponent->setSize(1024, 768);
  bure::engine::get().addEntity(std::move(backgroundEntity));

  // add button to start game
  auto ui = bure::engine::get().getUIManager();
  auto c = std::make_unique<bure::ui::container>(362, 309, 130, 60);
  c->setLayer(1);

  auto b = std::make_unique<bure::ui::button>("START GAME");
  b->set(20, 20, 90, 20);
  b->function = std::bind(&game::startGame, this, std::placeholders::_1);

  c->add(std::move(b));
  ui->addContainer(std::move(c));
}

void game::startGame(int unused) {
  // starts the actual game
  auto ui = bure::engine::get().getUIManager();
  ui->removeContainer(0);

  bure::engine::get().clearEntities();
  bure::engine::get().setMap("campo.tmx");

  // add game character
  auto characterEntity = std::make_unique<bure::entities::entity>();
  auto positionComponent =
      characterEntity->addComponent<bure::components::position_component>();
  characterEntity->addComponent<character_script_component>();
  characterEntity->setLayer(1);
  positionComponent->setCoords(496, 368);

  auto sprite1 = bure::components::sprite_component(*characterEntity);
  sprite1.setAll("personajes", { 95, 0, 32, 32 }, 32, 32);
  auto sprite2 = bure::components::sprite_component(*characterEntity);
  sprite2.setAll("personajes", { 159, 0, 32, 32 }, 32, 32);
  auto animation = characterEntity->addComponent<bure::components::animation_component>();
  animation->addSprite(sprite1);
  animation->addSprite(sprite2);
  animation->setAnimationTicks(20);

  bure::engine::get().addEntity(std::move(characterEntity));
}
