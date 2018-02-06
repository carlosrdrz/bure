#include "SDL.h"
#include "error.h"
#include "GUI/container.h"
#include "game.h"
#include "graphics.h"
#include "config.h"

#include <sstream>

typedef void (user_interface::*InterfaceFunc)(int param);

extern game *gameInstance;
extern graphics *pantalla;
extern config *configInstance;
extern player *playerInstance;

user_interface::user_interface() {
	// EMPEZAMOS CREANDO EL MENU PRINCIPAL
	// Crear el contenedor principal
	container *men = new container(362, 309, 300, 185);

	// Botones
	button *enviar = new button("ENTRAR");
		enviar->Set(25, 110, 70, 20);
		enviar->function = &user_interface::e_loadMap;

	men->Add(enviar);

    writing = true;
    blocked = false;

	this->addContainer(men); // Añadir el container a la UI
	containerOnFocus = containers.begin();
	(*containerOnFocus)->Focus();
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
	this->actualizarIndex();
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
		this->actualizarIndex();
	}
}

void user_interface::actualizarIndex() {
	std::list<container*>::iterator it;
	int in = 0;
	for (it = containers.begin(); it != containers.end(); ++it) {
		(*it)->index = in;
		in++;
	}

	if((int)containers.size() == 1) {
		containerOnFocus = containers.begin();
		(*containerOnFocus)->Focus();
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
    	(*containerOnFocus)->noFocus();
    }

	std::list<container*>::iterator it;
	for (it = containers.begin(); it != containers.end(); ++it) {
		if((*it)->index == num) {
			containerOnFocus = it;
			break;
		}
	}

	(*containerOnFocus)->Focus();
}

void user_interface::changeIBFocus(int num)
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

bool user_interface::clickOnIB(int x, int y)
{
	int z;
	if((int)containers.size() > 0) {
		for(z = 0; getContainerFocused()->getIB(z) != NULL; ++z) {
			if(x > (getContainerFocused()->x + getContainerFocused()->getIB(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getIB(z)->x + getContainerFocused()->getIB(z)->width) && y > (getContainerFocused()->y
			+ getContainerFocused()->getIB(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getIB(z)->y
			+ getContainerFocused()->getIB(z)->height))
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
		for(z = 0; getContainerFocused()->getBoton(z) != NULL; ++z) {
			if(x > (getContainerFocused()->x + getContainerFocused()->getBoton(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getBoton(z)->x + getContainerFocused()->getBoton(z)->width)&& y > (getContainerFocused()->y
			+ getContainerFocused()->getBoton(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getBoton(z)->y
			+ getContainerFocused()->getBoton(z)->height))
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
		for(z = 0; getContainerFocused()->getSelector(z) != NULL; ++z) {
			if(( x > (getContainerFocused()->x + getContainerFocused()->getSelector(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + 9) && y > (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y)
			&& y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) || ( x > (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 17) && x < (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 26) &&  y > (getContainerFocused()->y
			+ getContainerFocused()->getSelector(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) ) {
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
	for(z = 0; getContainerFocused()->getIB(z) != NULL; ++z) {
		if(x > (getContainerFocused()->x + getContainerFocused()->getIB(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getIB(z)->x + getContainerFocused()->getIB(z)->width)&& y > (getContainerFocused()->y
		+ getContainerFocused()->getIB(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getIB(z)->y
		+ getContainerFocused()->getIB(z)->height))
		{
			toreturn = getContainerFocused()->getIB(z);
		}
	}
	return toreturn;
}

button *user_interface::getButtonClicked(int x, int y)
{
	int z;
	button *toreturn = NULL;
	for(z = 0; getContainerFocused()->getBoton(z) != NULL; ++z) {
		if(x > (getContainerFocused()->x + getContainerFocused()->getBoton(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getBoton(z)->x + getContainerFocused()->getBoton(z)->width)&& y > (getContainerFocused()->y
		+ getContainerFocused()->getBoton(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getBoton(z)->y
		+ getContainerFocused()->getBoton(z)->height))
		{
			toreturn = getContainerFocused()->getBoton(z);
		}
	}
	return toreturn;
}

ui_selector *user_interface::getSelectorClicked(int x, int y)
{
	int z;
	ui_selector *toreturn = NULL;
	for(z = 0; getContainerFocused()->getSelector(z) != NULL; ++z) {
		if(( x > (getContainerFocused()->x + getContainerFocused()->getSelector(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + 9) && y > (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y)
		&& y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) || ( x > (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 17) && x < (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 26) &&  y > (getContainerFocused()->y
		+ getContainerFocused()->getSelector(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) ) {
			toreturn = getContainerFocused()->getSelector(z);
		}
	}
	return toreturn;
}

//////////////////////////
// ACCIONES DE LOS BOTONES
//////////////////////////
void user_interface::ejecutarBoton(button *a_ejecutar)
{
    InterfaceFunc ejec;
    ejec = a_ejecutar->function;
    (this->*ejec)(a_ejecutar->parametro);
}

void user_interface::ejecutarBoton(input_box *a_ejecutar)
{
    InterfaceFunc ejec;
    ejec = a_ejecutar->function;
    (this->*ejec)(a_ejecutar->parametro);
}


void user_interface::e_cerrarJuego(int param)
{
    gameInstance->cerrar();
}

void user_interface::e_loadMap(int param)
{
	gameInstance->cambiarMapa("campo.tmx");
	playerInstance->setPosition(30, 36);
    gameInstance->playing = true;
    this->writing = false;
    this->closeContainer(0);
}