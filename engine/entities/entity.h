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
  virtual ~entity() = default;

  virtual void init() {}
  virtual void update() {}
  void destroy();

  entity* getParent() const;
  std::list<entity*> getChildren() const;
  void addChild(std::unique_ptr<entity> e);
  void removeChild(entity *e);

  void setLayer(int layer);
  int getLayer();

  template <class T>
  T* addComponent() {
    auto ptr = std::make_unique<T>(*this);
    auto toReturn = ptr.get();
    _components.push_back(std::move(ptr));
    toReturn->onInit();
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

  template <class T>
  void removeComponentByType() {
    auto it = _components.begin();

    while (it != _components.end()) {
      if (T* componentType = dynamic_cast<T*>((*it).get())) {
        it = _components.erase(it);
      } else {
        ++it;
      }
    }
  }

 private:
  std::list<std::unique_ptr<bure::components::component>> _components;
  std::list<std::unique_ptr<entity>> _children;
  entity *_parent;

  // higher layers means it will be rendered later
  int _layer = 0;
};
}  // namespace entities
}  // namespace bure
