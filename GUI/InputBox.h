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

#ifndef __H_GUI_INPUTBOX__
#define __H_GUI_INPUTBOX__

#include "Widget.h"
#include "../Interface.h"
#include <string>

class Interface;

typedef void (Interface::*InterfaceFunc)(int param);

class InputBox : public Widget {
public:
	InterfaceFunc function;
	std::string texto;
	std::string titulo;
	int parametro;
	int secreto;
	
	InputBox();
	InputBox(std::string title, std::string prf, int maxchars = 12);
	~InputBox() {}

	void Set(int x, int y, int anc);
	
	void Escribir(char que);
	void Borrar();

	void EsSecreto() { this->secreto = 1; }
	bool escapeable() { return charsplus; }
	void escapeable(bool que) { charsplus = que; }
	
	void Focus() { focus = 1; }
	void noFocus() { focus = 0; }
	int getFocus() { return focus; }
	
	const char *getTexto() { return texto.c_str(); }
	const char *getTitulo() { return titulo.c_str(); }
	int getCaracteres() { return (int)texto.size(); }
	
private:
	int focus;
	int maxchar;
	bool charsplus;
};

#endif
