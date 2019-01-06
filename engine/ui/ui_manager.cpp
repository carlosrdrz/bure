#include <functional>
#include <set>

#include "ui_manager.h"

namespace bure {
namespace ui {

void ui_manager::addContainer(std::unique_ptr<container> c) {
  c->index = static_cast<int>(_containers.size());
  _containers.push_back(std::move(c));
}

void ui_manager::removeContainer(int in) {
  std::vector<std::unique_ptr<container>>::iterator it;

  for (it = _containers.begin(); it != _containers.end(); ++it) {
    if ((*it)->index == in) {
      (*it)->deinit();
      _containers.erase(it);
      break;
    }
  }

  this->updateIndex();
}

void ui_manager::removeContainer(container* p) {
  if (nullptr != p) {
    std::vector<std::unique_ptr<container>>::iterator it;

    for (it = _containers.begin(); it != _containers.end(); ++it) {
      if ((*it).get() == p) {
        (*it)->deinit();
        _containers.erase(it);
        break;
      }
    }

    this->updateIndex();
  }
}

void ui_manager::updateIndex() {
  int in = 0;

  for (auto& c : _containers) {
    c->index = in++;
  }
}

container* ui_manager::getContainer(int cont) {
  if (cont > static_cast<int>(_containers.size()) - 1) return nullptr;

  for (auto&& c : _containers) {
    if (c->index == cont) {
      return c.get();
    }
  }

  return nullptr;
}

bool ui_manager::containerExists(container* p) {
  for (auto&& c : _containers) {
    if (c.get() == p) {
      return true;
    }
  }

  return false;
}

std::vector<std::reference_wrapper<container>> ui_manager::getContainers() {
  std::vector<std::reference_wrapper<container>> conts;

  for (auto& c : _containers) {
    conts.emplace_back(std::ref(*c));
  }

  return conts;
}

}  // namespace ui
}  // namespace bure
