#pragma once

#include "../engine/event_manager.h"

namespace bure {
namespace ui {

class widget {
 public:
  int width, height, x, y, index;

  int getAbsoluteX();
  int getAbsoluteY();
  bool wasClicked(int x, int y);

  void setParent(widget* w);
  widget* getParent();
  widget *parent;
};

}  // namespace ui
}  // namespace bure
