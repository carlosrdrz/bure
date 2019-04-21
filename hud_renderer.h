#pragma once

#include "components/position_component.h"
#include "components/stats_component.h"
#include "components/component.h"
#include "renderer.h"

using namespace bure::components;

class hud_renderer : public bure::renderer {
 public:
  void render(int layer);

 private:
  void renderHealthBars(position_component &p, stats_component &s);
};
