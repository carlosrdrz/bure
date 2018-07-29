#pragma once

#include <vector>
#include <memory>

#include "button.h"
#include "image.h"
#include "input_box.h"
#include "label.h"
#include "selector.h"
#include "widget.h"

namespace bure {
namespace ui {

class container : public widget {
 public:
  int visible;
  int index;
  bool buttonPressed;
  bool focused;

  container();
  container(int x, int y, int w, int h);
  ~container();

  void add(std::unique_ptr<label> w);
  void add(std::unique_ptr<input_box> w);
  void add(std::unique_ptr<button> w);
  void add(std::unique_ptr<image> w);
  void add(std::unique_ptr<selector> w);

  input_box* get_input_box(int ib);
  label* get_label(int lb);
  button* get_button(int b);
  image* get_image(int im);
  selector* get_selector(int sel);

 private:
  std::vector<std::unique_ptr<label>> labels;
  std::vector<std::unique_ptr<input_box>> inputBoxes;
  std::vector<std::unique_ptr<button>> buttons;
  std::vector<std::unique_ptr<image>> images;
  std::vector<std::unique_ptr<selector>> selectors;

  void onClickUp(SDL_Event e);
};

}  // namespace ui
}  // namespace bure
