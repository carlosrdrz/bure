#pragma once

#include "event.h"

namespace bure {
namespace events {

class event_close : public event {
 public:
  event_id getID() const { return event_id::close; }
};

}  // namespace events
}  // namespace bure
