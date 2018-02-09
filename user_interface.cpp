#include "SDL.h"
#include "error.h"
#include "ui/container.h"
#include "game.h"
#include "graphics.h"
#include "config.h"

#include <sstream>

extern game *gameInstance;
extern graphics *pantalla;
extern config *configInstance;
extern player *playerInstance;

using namespace std::placeholders;
using namespace ui;

user_interface::user_interface() {
    // Main menu
    auto men = new ui::container(362, 309, 130, 60);

    // Buttons
    auto enviar = ui::button("START GAME");
    enviar.set(20, 20, 90, 20);
    enviar.function = bind(&user_interface::e_loadMap, this, _1);

    men->add(enviar);

    writing = true;
    blocked = false;

    this->addContainer(men);
    containerOnFocus = containers.begin();
    (*containerOnFocus)->focused = true;
}

user_interface::~user_interface() {
    containers.clear();
}

void user_interface::addContainer(container *e) {
    e->index = (int)containers.size();
    containers.push_back(e);
    if((int)containers.size() == 1) changeContainerFocus(0);
}

void user_interface::closeContainer(int in) {
    std::list<container*>::iterator it;

    for (it = containers.begin(); it != containers.end(); ++it) {
        if((*it)->index == in) {
            if((*it)->index == getContainerFocused()->index) {
                changeContainerFocus(getContainerFocused()->index-1);
            }

            containers.erase(it);
            break;
        }
    }
    this->updateIndex();
}

void user_interface::closeContainer(container *p) {
    if(NULL != p) {
        std::list<container*>::iterator it;

        for (it = containers.begin(); it != containers.end(); ++it) {
            if((*it) == p) {
                if((*it) == getContainerFocused()) {
                    changeContainerFocus(getContainerFocused()->index-1);
                }

                containers.erase(it);
                break;
            }
        }
        this->updateIndex();
    }
}

void user_interface::updateIndex() {
    std::list<container*>::iterator it;
    int in = 0;
    for (it = containers.begin(); it != containers.end(); ++it) {
        (*it)->index = in;
        in++;
    }

    if((int)containers.size() == 1) {
        containerOnFocus = containers.begin();
        (*containerOnFocus)->focused = true;
    }
}

container *user_interface::getContainer(int cont) {
    std::list<container*>::iterator it;

    if(cont > (int)containers.size()-1) return NULL;

    for(it = containers.begin(); it != containers.end(); ++it) {
        if((*it)->index == cont) {
            return (*it);
        }
    }

    return NULL;
}

bool user_interface::containerExists(container *p) {
    bool res = false;
    std::list<container*>::iterator it;

    if(NULL != p) {
        for(it = containers.begin(); it != containers.end(); ++it) {
            if((*it) == p) {
                res = true;
            }
        }
    }

    return res;
}

void user_interface::changeContainerFocus(int num)
{
    if((int)containers.size() > 1) {
        (*containerOnFocus)->focused = false;
    }

    std::list<container*>::iterator it;
    for (it = containers.begin(); it != containers.end(); ++it) {
        if((*it)->index == num) {
            containerOnFocus = it;
            break;
        }
    }

    (*containerOnFocus)->focused = true;
}

void user_interface::changeInputBoxFocus(int num)
{
    (*containerOnFocus)->changeIBFocus(num);
}

void user_interface::changeButtonFocus(int num)
{
    (*containerOnFocus)->changeButtonFocus(num);
}

container *user_interface::getContainerFocused()
{
    if((int)containers.size() == 0) return NULL;
    return (*containerOnFocus);
}

input_box *user_interface::getInputBoxFocused()
{
    if((int)containers.size() == 0) return NULL;
    return (*containerOnFocus)->getIBFocused();
}

button *user_interface::getButtonFocused()
{
    return (*containerOnFocus)->getButtonFocused();
}

bool user_interface::clickOnContainer(int x, int y)
{
    if((int)containers.size() > 0) {
        std::list<container*>::iterator itib;
        for(itib = containers.begin(); itib != containers.end(); ++itib) {
            if(x > (*itib)->x && x < ((*itib)->x + (*itib)->width) && y > (*itib)->y
            && y < ((*itib)->y + (*itib)->height)) {
                return true;
            }
        }
    }
    return false;
}

bool user_interface::clickOnInputBox(int x, int y)
{
    int z;
    if((int)containers.size() > 0) {
        for(z = 0; getContainerFocused()->get_input_box(z) != NULL; ++z) {
            if(x > (getContainerFocused()->x + getContainerFocused()->get_input_box(z)->x) && x < (getContainerFocused()->x
            + getContainerFocused()->get_input_box(z)->x + getContainerFocused()->get_input_box(z)->width) && y > (getContainerFocused()->y
            + getContainerFocused()->get_input_box(z)->y) && y < (getContainerFocused()->y +
                    getContainerFocused()->get_input_box(z)->y
            + getContainerFocused()->get_input_box(z)->height))
            {
                return true;
            }
        }
    }
    return false;
}

bool user_interface::clickOnButton(int x, int y)
{
    int z;
    if((int)containers.size() > 0) {
        for(z = 0; getContainerFocused()->get_button(z) != NULL; ++z) {
            if(x > (getContainerFocused()->x + getContainerFocused()->get_button(z)->x) && x < (getContainerFocused()->x
            + getContainerFocused()->get_button(z)->x + getContainerFocused()->get_button(z)->width)&& y > (getContainerFocused()->y
            + getContainerFocused()->get_button(z)->y) && y < (getContainerFocused()->y +
                    getContainerFocused()->get_button(z)->y
            + getContainerFocused()->get_button(z)->height))
            {
                return true;
            }
        }
    }
    return false;
}

bool user_interface::clickOnSelector(int x, int y)
{
    int z;
    if(containers.size() > 0) {
        for(z = 0; getContainerFocused()->get_selector(z) != NULL; ++z) {
            if(( x > (getContainerFocused()->x + getContainerFocused()->get_selector(z)->x) && x < (getContainerFocused()->x
            + getContainerFocused()->get_selector(z)->x + 9) && y > (getContainerFocused()->y +
                    getContainerFocused()->get_selector(z)->y)
            && y < (getContainerFocused()->y + getContainerFocused()->get_selector(z)->y + 14)) || ( x > (getContainerFocused()->x
            + getContainerFocused()->get_selector(z)->x + getContainerFocused()->get_selector(z)->width + 17) && x < (getContainerFocused()->x
            + getContainerFocused()->get_selector(z)->x + getContainerFocused()->get_selector(z)->width + 26) &&  y > (getContainerFocused()->y
            + getContainerFocused()->get_selector(z)->y) && y < (getContainerFocused()->y +
                    getContainerFocused()->get_selector(z)->y + 14)) ) {
                return true;
            }
            /*

            */
        }
    }
    return false;
}

container *user_interface::getContainerClicked(int x, int y)
{
    std::list<container*>::iterator itib, toreturn;
    toreturn = containers.end();
    for(itib = containers.begin(); itib != containers.end(); ++itib) {
        if(x > (*itib)->x && x < ((*itib)->x + (*itib)->width) && y > (*itib)->y
        && y < ((*itib)->y + (*itib)->height)) {
            toreturn = itib;
        }
    }

    if(toreturn == containers.end()) return NULL;

    return (*toreturn);
}

input_box *user_interface::getInputBoxClicked(int x, int y)
{
    int z;
    input_box *toreturn = NULL;
    for(z = 0; getContainerFocused()->get_input_box(z) != NULL; ++z) {
        if(x > (getContainerFocused()->x + getContainerFocused()->get_input_box(z)->x) && x < (getContainerFocused()->x
        + getContainerFocused()->get_input_box(z)->x + getContainerFocused()->get_input_box(z)->width)&& y > (getContainerFocused()->y
        + getContainerFocused()->get_input_box(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->get_input_box(z)->y
        + getContainerFocused()->get_input_box(z)->height))
        {
            toreturn = getContainerFocused()->get_input_box(z);
        }
    }
    return toreturn;
}

button *user_interface::getButtonClicked(int x, int y)
{
    int z;
    button *toreturn = NULL;
    for(z = 0; getContainerFocused()->get_button(z) != NULL; ++z) {
        if(x > (getContainerFocused()->x + getContainerFocused()->get_button(z)->x) && x < (getContainerFocused()->x
        + getContainerFocused()->get_button(z)->x + getContainerFocused()->get_button(z)->width)&& y > (getContainerFocused()->y
        + getContainerFocused()->get_button(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->get_button(z)->y
        + getContainerFocused()->get_button(z)->height))
        {
            toreturn = getContainerFocused()->get_button(z);
        }
    }
    return toreturn;
}

selector *user_interface::getSelectorClicked(int x, int y)
{
    int z;
    selector *toreturn = NULL;
    for(z = 0; getContainerFocused()->get_selector(z) != NULL; ++z) {
        if(( x > (getContainerFocused()->x + getContainerFocused()->get_selector(z)->x) && x < (getContainerFocused()->x
        + getContainerFocused()->get_selector(z)->x + 9) && y > (getContainerFocused()->y +
                getContainerFocused()->get_selector(z)->y)
        && y < (getContainerFocused()->y + getContainerFocused()->get_selector(z)->y + 14)) || ( x > (getContainerFocused()->x
        + getContainerFocused()->get_selector(z)->x + getContainerFocused()->get_selector(z)->width + 17) && x < (getContainerFocused()->x
        + getContainerFocused()->get_selector(z)->x + getContainerFocused()->get_selector(z)->width + 26) &&  y > (getContainerFocused()->y
        + getContainerFocused()->get_selector(z)->y) && y < (getContainerFocused()->y +
                getContainerFocused()->get_selector(z)->y + 14)) ) {
            toreturn = getContainerFocused()->get_selector(z);
        }
    }
    return toreturn;
}

//////////////////////////
// ACCIONES DE LOS BOTONES
//////////////////////////
void user_interface::execButton(button *button)
{
    button->function(button->parameter);
}

void user_interface::e_loadMap(int param)
{
    gameInstance->cambiarMapa("campo.tmx");
    playerInstance->setPosition(30, 36);
    gameInstance->playing = true;
    this->writing = false;
    this->closeContainer(0);
}