#include "movement_system.h"

#include "engine.h"

void movement_system::update() {
  for (auto &entity : bure::engine::get().getEntities()) {
    auto &e = entity.get();

    auto pos = e.getComponentByType<position_component>();
    auto mv = e.getComponentByType<movement_component>();
    auto s = e.getComponentByType<solid_component>();
    if (mv == nullptr || pos == nullptr || s == nullptr) continue;

    moveEntity(e, *pos, *mv, *s);
  }
}

void movement_system::moveEntity(entity &e, position_component &p,
                                 movement_component &m, solid_component &s) {
  auto map = bure::engine::get().getMap();
  auto layer = map->getLayer(2);
  auto nextPos = bure::world_coords(p.getPosition());
  bool isFinished = false;
  bure::map_coords nextPosition = m.getPosition();

  switch (m.getState()) {
    case movement_state::walking_left:
      nextPosition.x -= 1;
      nextPos.x -= m.getVelocity();
      isFinished = nextPos.x % map->getTileWidth() == 0;
      break;
    case movement_state::walking_right:
      nextPosition.x += 1;
      nextPos.x += m.getVelocity();
      isFinished = nextPos.x % map->getTileWidth() == 0;
      break;
    case movement_state::walking_up:
      nextPosition.y -= 1;
      nextPos.y -= m.getVelocity();
      isFinished = nextPos.y % map->getTileHeight() == 0;
      break;
    case movement_state::walking_down:
      nextPosition.y += 1;
      nextPos.y += m.getVelocity();
      isFinished = nextPos.y % map->getTileHeight() == 0;
      break;
    default:
      break;
  }

  p.setPosition(nextPos);

  if (isFinished) {
    setPosition(e, p, m, s, nextPosition);
    stopMoving(m);
  }
}

void movement_system::stopMoving(movement_component &m) {
  switch (m.getState()) {
    case movement_state::walking_left:
      m.setState(movement_state::standing_left);
      break;
    case movement_state::walking_right:
      m.setState(movement_state::standing_right);
      break;
    case movement_state::walking_up:
      m.setState(movement_state::standing_up);
      break;
    case movement_state::walking_down:
      m.setState(movement_state::standing_down);
      break;
    default:
      break;
  }
}

void movement_system::setPosition(entity &e, position_component &p,
                                  movement_component &m, solid_component &s,
                                  bure::map_coords pos) {
  auto map = bure::engine::get().getMap();
  auto wc = map->mapToWorld(pos);
  p.setPosition(wc);
  m.setPosition(pos);
  s.setPosition(pos);
}
