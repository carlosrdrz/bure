#include "selector.h"

#include <list>
#include <string>

ui::selector::selector(int x, int y)
{
	opciones.push_back("DEFAULT");
	opcionesit = opciones.begin();
	this->x = x;
	this->y = y;
	this->height = 14;
	this->width = 0;
}

ui::selector::~selector()
{
	opciones.clear();
}

void ui::selector::addOption(std::string toadd)
{
	if(opciones.front().compare("DEFAULT") == 0) {
		opciones.clear();
	}
	opciones.push_back(toadd);
}

std::string ui::selector::getSelected()
{
	return (*opcionesit);
}

void ui::selector::next()
{
	if(opciones.size() > 1) {
		if((*opcionesit).compare(opciones.back()) == 0) opcionesit = opciones.begin();
		else opcionesit++;
	}
}

void ui::selector::previous()
{
	if(opciones.size() > 1) {
		if(opcionesit == opciones.begin()) {
			opcionesit = opciones.end();
			opcionesit--;
		} else {
			opcionesit--;
		}
	}
}
