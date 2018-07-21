#pragma once

namespace bure {
namespace events {

enum event_id { close, keyboard };

class event {
 public:
  virtual event_id getID() const = 0;
};

}  // namespace events
}  // namespace bure
