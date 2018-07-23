#pragma once

#include <vector>
#include <memory>

#include "button.h"
#include "container.h"
#include "input_box.h"
#include "selector.h"
#include "../engine/event_manager.h"
#include "../engine/utils/logger.h"

namespace bure {
namespace ui {

class ui_manager {
 public:
  ui_manager();
  ~ui_manager();

  bool writing;
  bool blocked;

  void addContainer(std::unique_ptr<container> e);

  void closeContainer(int in);
  void closeContainer(ui::container *p);

  void changeContainerFocus(int num);
  void changeInputBoxFocus(int num);
  void changeButtonFocus(int num);

  bool clickOnContainer(int x, int y);
  bool clickOnInputBox(int x, int y);
  bool clickOnButton(int x, int y);
  bool clickOnSelector(int x, int y);

  bool containerExists(ui::container* p);

  ui::container* getContainer(int cont);
  ui::container* getContainerClicked(int x, int y);
  ui::input_box* getInputBoxClicked(int x, int y);
  ui::button* getButtonClicked(int x, int y);
  ui::selector* getSelectorClicked(int x, int y);

  ui::container* getContainerFocused();
  ui::input_box* getInputBoxFocused();
  ui::button* getButtonFocused();

  // Button actions
  void execButton(ui::button* button);

 private:
  std::vector<std::unique_ptr<container>> _containers;
  container* containerOnFocus;

  void updateIndex();
  void onClickDown(SDL_Event e);
  void onClickUp(SDL_Event e);
};

}  // namespace ui
}  // namespace bure
