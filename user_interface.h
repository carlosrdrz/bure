#pragma once

#include "ui/button.h"
#include "ui/container.h"
#include "ui/input_box.h"
#include "ui/selector.h"

#include <list>

class user_interface {
 public:
        user_interface();
        ~user_interface();

        bool writing;
        bool blocked;

        void addContainer(ui::container *e);
        void closeContainer(int in);
        void closeContainer(ui::container *p);

        void changeContainerFocus(int num);
        void changeInputBoxFocus(int num);
        void changeButtonFocus(int num);

        bool clickOnContainer(int x, int y);
        bool clickOnInputBox(int x, int y);
        bool clickOnButton(int x, int y);
        bool clickOnSelector(int x, int y);

        bool containerExists(ui::container *p);
        ui::container *getContainer(int cont);

        ui::container *getContainerClicked(int x, int y);
        ui::input_box *getInputBoxClicked(int x, int y);
        ui::button *getButtonClicked(int x, int y);
        ui::selector *getSelectorClicked(int x, int y);

        ui::container *getContainerFocused();
        ui::input_box *getInputBoxFocused();
        ui::button *getButtonFocused();

        // Button actions
        void execButton(ui::button *button);

 private:
        std::list<ui::container*> containers;
        std::list<ui::container*>::iterator containerOnFocus;

        void updateIndex();
};
