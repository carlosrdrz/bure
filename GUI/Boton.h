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

#ifndef __H_GUI_BOTON__
#define __H_GUI_BOTON__

#include "Widget.h"
#include "../Interface.h"
#include <string>

class Interface;

typedef void (Interface::*InterfaceFunc)(int param);

class Boton : public Widget {
public:
	std::string titulo;
	InterfaceFunc function;
	int press;	
	int parametro;

	Boton() { this->press = 0; this->parametro = 0; }
	Boton(std::string title) {
		this->titulo = title;
		this->press = 0;
		this->parametro = 0;
	}
	~Boton() {}

	const char *getTitulo() { return titulo.c_str(); }

	void Set(int x, int y, int anc, int larg) {
		this->x = x;
		this->y = y;
		this->ancho = anc;
		this->alto = larg;
	}
};

#endif
