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

  void removeContainer(int in);
  void removeContainer(container *p);

  bool containerExists(container* p);

  container* getContainer(int cont);
  std::vector<std::reference_wrapper<container>> getContainers();

  void setScale(int s);
  int getScale();

 private:
  std::vector<std::unique_ptr<container>> _containers;
  int _scale = 1;

  void updateIndex();
};

}  // namespace ui
}  // namespace bure
