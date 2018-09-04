#pragma once

#include "../engine/event_manager.h"

namespace bure {
namespace ui {

class widget {
 public:
  widget() : width(0), height(0), x(0), y(0), index(0), parent(nullptr) {}

  int width, height, x, y, index;

  int getAbsoluteX();
  int getAbsoluteY();
  bool wasClicked(int x, int y);

  void setParent(widget* w);
  widget* getParent();
  widget* parent = nullptr;
};

}  // namespace ui
}  // namespace bure
