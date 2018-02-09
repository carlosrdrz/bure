#pragma once

#include "widget.h"

#include <list>
#include <string>

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

 private:
    std::list<std::string> options;
};
};