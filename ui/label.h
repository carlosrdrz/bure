#pragma once

#include <string>

#include "widget.h"

namespace bure {
namespace ui {

class label : public widget {
 public:
  explicit label(std::string title, int size = 16);
  ~label() = default;

  void set(int x, int y);

  const char *get_text() { return text.c_str(); }
  void set_text(std::string que);

  int get_size() { return size; }

 private:
  std::string text;
  int size;
};

}  // namespace ui
}  // namespace bure
