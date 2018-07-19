# pragma once

namespace bure {
namespace events {

using event_id = std::string;

class event {
 public:
  virtual event_id getID() const = 0;
};

}  // namespace events
}  // namespace bure
