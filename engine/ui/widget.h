#pragma once

#include "event_manager.h"

namespace bure {
namespace ui {

enum class widget_type {
  widget,
  container,
  button,
  input_box,
  image,
  label,
  selector
};

class widget {
 public:
  widget() : index(0), x(0), y(0), width(0), height(0) {}

  // TODO(carlosrdrz): This probably should not be a public member
  int index;

  // Called when the widget is added to a container
  // Do initialization stuff here like adding event handlers
  virtual void init() {}

  int getRelativeX() const;
  int getRelativeY() const;
  int getAbsoluteX() const;
  int getAbsoluteY() const;
  int getWidth() const;
  int getHeight() const;

  bool wasClicked(int x, int y) const;

  void setParent(widget* w);
  widget* getParent() const;
  virtual widget_type getType() const;

 protected:
  int x, y, width, height;
  widget* parent = nullptr;
};

}  // namespace ui
}  // namespace bure
