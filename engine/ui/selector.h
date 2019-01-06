#pragma once

#include <list>
#include <string>

#include "widget.h"

namespace bure {
namespace ui {

class selector : public widget {
 public:
  selector(int x, int y);
  ~selector();

  void add_option(std::string value);

  std::string get_selected();

  void next();
  void previous();

  std::list<std::string>::iterator options_it;
  widget_type getType() const override { return widget_type::selector; }

 private:
  std::list<std::string> options;
};

}  // namespace ui
}  // namespace bure
