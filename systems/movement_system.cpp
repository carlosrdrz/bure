#include "movement_system.h"

#include "engine.h"

void movement_system::update() {
  for (auto &entity : bure::engine::get().getEntities()) {
    auto &e = entity.get();

    auto pos = e.getComponentByType<position_component>();
    auto mv = e.getComponentByType<movement_component>();
    if (mv == nullptr || pos == nullptr) continue;

    moveEntity(e, *pos, *mv);
  }
}

void movement_system::moveEntity(entity &e, position_component &p,
                                 movement_component &m) {
  auto map = bure::engine::get().getMap();
  auto nextPos = p.getPosition();
  bool isFinished = false;

  switch (m.getDirection()) {
    case bure::direction::left:
      nextPos.x -= m.getVelocity();
      isFinished = nextPos.x % map->getTileWidth() == 0;
      break;
    case bure::direction::right:
      nextPos.x += m.getVelocity();
      isFinished = nextPos.x % map->getTileWidth() == 0;
      break;
    case bure::direction::up:
      nextPos.y -= m.getVelocity();
      isFinished = nextPos.y % map->getTileHeight() == 0;
      break;
    case bure::direction::down:
      nextPos.y += m.getVelocity();
      isFinished = nextPos.y % map->getTileHeight() == 0;
      break;
    default:
      break;
  }

  p.setPosition(nextPos);

  if (isFinished) {
    m.setDirection(bure::direction::none);
  }
}
