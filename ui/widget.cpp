#include "widget.h"

#include "../engine/utils/logger.h"

namespace bure {
namespace ui {

int widget::getAbsoluteX() {
  if (parent == nullptr) return x;
  return parent->getAbsoluteX() + x;
}

int widget::getAbsoluteY() {
  if (parent == nullptr) return y;
  return parent->getAbsoluteY() + y;
}

bool widget::wasClicked(int clickX, int clickY) {
  auto x = getAbsoluteX();
  auto y = getAbsoluteY();

  return (clickX > x && clickX < (x + width) &&
          clickY > y && clickY < (y + height));
}

void widget::setParent(widget* w) {
  parent = w;
}

widget* widget::getParent() {
  return parent;
}

}  // namespace ui
}  // namespace bure
