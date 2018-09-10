#pragma once

#include <vector>
#include <memory>

#include "button.h"
#include "container.h"
#include "event_manager.h"
#include "utils/logger.h"

namespace bure {
namespace ui {

class ui_manager {
 public:
  void addContainer(std::unique_ptr<container> c);

  void closeContainer(int in);
  void closeContainer(ui::container *p);

  bool containerExists(ui::container* p);

  ui::container* getContainer(int cont);
  std::vector<std::reference_wrapper<container>> getContainers();

 private:
  std::vector<std::unique_ptr<container>> _containers;

  void updateIndex();
};

}  // namespace ui
}  // namespace bure
