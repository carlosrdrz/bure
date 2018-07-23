#pragma once

#include <memory>
#include <list>

#include "../components/component.h"

namespace bure {

namespace components {
class component;
}

namespace entities {

typedef unsigned int entity_id;

class entity {
 public:
  entity() : _parent(nullptr) {}

  void init() {}
  void update() {}
  void destroy();

  entity* getParent() const;
  std::list<entity*> getChildren() const;
  void addChild(std::unique_ptr<entity> e);
  void removeChild(entity *e);

  template <class T>
  T* addComponent() {
    auto ptr = std::make_unique<T>(*this);
    auto toReturn = ptr.get();
    _components.push_back(std::move(ptr));
    return toReturn;
  }

  template <class T>
  T* getComponentByType() {
    for (auto& c : _components) {
      if (T* componentType = dynamic_cast<T*>(c.get())) {
        return componentType;
      }
    }
    return nullptr;
  }

 private:
  std::list<std::unique_ptr<bure::components::component>> _components;
  std::list<std::unique_ptr<entity>> _children;
  entity *_parent;
};
}  // namespace entities
}  // namespace bure
