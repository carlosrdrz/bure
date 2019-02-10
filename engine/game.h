#pragma once

namespace bure {
class game {
 public:
  virtual void init(){};
  virtual void finish();

  bool isFinished();

 private:
  bool _finished;
};
}  // namespace bure
