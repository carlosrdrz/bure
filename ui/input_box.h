#pragma once

#include <functional>
#include <string>

#include "widget.h"

namespace bure {
namespace ui {

class input_box : public widget {
 public:
  using callback = std::function<void(int)>;

  bool focused = false;
  callback function;
  std::string text;
  std::string title;

  input_box();
  input_box(std::string title, std::string text, int max_length = 12);
  ~input_box() = default;

  void set(int x, int y, int w);
  void write(char que);
  void remove_last();

  const char *get_text() { return text.c_str(); }
  const char *get_title() { return title.c_str(); }

  int current_length() { return static_cast<int>(text.size()); }

 private:
  int max_length;
};

}  // namespace ui
}  // namespace bure
