//////////////////////////////////////////////////////////////////////////
//  Bure Online 0.2.0
//  Copyright (C) 2010 Carlos Rodriguez Reyes
//////////////////////////////////////////////////////////////////////////
//
//  Bure Online is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Bure Online is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Bure Online. If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////

#include "Selector.h"

#include <list>
#include <string>

Selector::Selector(int x, int y)
{
	opciones.push_back("DEFAULT");
	opcionesit = opciones.begin();
	this->x = x;
	this->y = y;
	this->alto = 14;
	this->ancho = 0;
}

Selector::~Selector()
{
	opciones.clear();
}

void Selector::addOption(std::string toadd)
{
	if(opciones.front().compare("DEFAULT") == 0) {
		opciones.clear();
	}
	opciones.push_back(toadd);
}

std::string Selector::getSelected()
{
	return (*opcionesit);
}

void Selector::next()
{
	if(opciones.size() > 1) {
		if((*opcionesit).compare(opciones.back()) == 0) opcionesit = opciones.begin();
		else opcionesit++;
	}
}

void Selector::previous()
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
