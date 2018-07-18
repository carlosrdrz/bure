#pragma once

#include <list>
#include <functional>
#include <map>

#include "SDL.h"
#include "events/event.h"

namespace bure {

class event_manager {
 public:
    template<class T>
    void addEventCallback(std::function<void(T)>);

 private:
    std::map<events::event_id, std::list<std::function<void(events::event)>>> _eventCallbacks;
};

}  // namespace bure
