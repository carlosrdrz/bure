# pragma once

#include "event.h"

namespace bure {
namespace events {

class event_keyboard : public event {
 public:
  event_id getID() const { return "keyboard"; }

};

}  // namespace events
}  // namespace bure
