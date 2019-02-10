#include "hud_renderer.h"

#include "engine.h"
#include "utils/logger.h"

namespace bure {

void hud_renderer::render(int layer) {
  for (auto& entity : bure::engine::get().getEntities()) {
    auto& e = entity.get();

    if (e.getLayer() != layer) continue;

    auto position = e.getComponentByType<position_component>();
    if (position == nullptr) continue;

    auto stats = e.getComponentByType<stats_component>();
    if (stats != nullptr) {
      renderHealthBars(*position, *stats);
      continue;
    }
  }
}

void hud_renderer::renderHealthBars(position_component& p, stats_component& s) {
  auto camera = bure::engine::get().getCamera();
  auto position = p.getPosition();
  auto maxHP = s.getMaxHP();
  auto currentHP = s.getHP();
  auto pct = ((double) currentHP / maxHP);
  auto color = bure::color({ 0, 0, 0, SDL_ALPHA_OPAQUE });
  bure::rect dst = {
    position.x - camera.x + 8,
    position.y - camera.y - 10,
    50,
    6
  };

  _graphics->drawRect(dst, color);

  color = bure::color({ 0, 179, 44, SDL_ALPHA_OPAQUE });
  dst = {
    position.x - camera.x + 9,
    position.y - camera.y - 9,
    (int) (48 * pct),
    4
  };

  _graphics->drawRect(dst, color);
}

}  // namespace bure
