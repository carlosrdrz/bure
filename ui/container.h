#pragma once

#include "widget.h"
#include "button.h"
#include "input_box.h"
#include "label.h"
#include "image.h"
#include "selector.h"

#include <list>

namespace ui {
class container : public widget {
 public:
    int visible;
    int index;
    bool buttonPressed;
    bool focused;

    container();
    container(int x, int y, int w, int h);
    ~container();

    void add(label w);
    void add(input_box w);
    void add(button w);
    void add(image w);
    void add(selector w);

    input_box* get_input_box(int ib);
    label* get_label(int lb);
    button* get_button(int b);
    image* get_image(int im);
    selector* get_selector(int sel);

    void changeIBFocus(int num);
    void changeButtonFocus(int num);

    input_box* getIBFocused() { return inputBoxOnFocus; }
    button* getButtonFocused() { return buttonOnFocus; }

 private:
    std::list<label> labels;
    std::list<input_box> inputBoxes;
    std::list<button> buttons;
    std::list<image> images;
    std::list<selector> selectors;

    input_box* inputBoxOnFocus = nullptr;
    button* buttonOnFocus = nullptr;
};
};