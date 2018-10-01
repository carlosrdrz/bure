#pragma once

#include <string>

#include "widget.h"

namespace bure {
namespace ui {

class label : public widget {
 public:
  explicit label(std::string title, int size = 16);

  std::string getText() const { return text; }
  int getSize() const { return size; }

  void set(int x, int y);
  void setText(std::string que);
  widget_type getType() const { return widget_type::label; }

 private:
  std::string text;
  int size;
};

}  // namespace ui
}  // namespace bure
