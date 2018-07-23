#pragma once

namespace bure {
namespace systems {

typedef unsigned int system_id;

class system {
 public:
    virtual void init() {}
    virtual void update() {}
    virtual void release() {}
};

}  // namespace systems
}  // namespace bure
