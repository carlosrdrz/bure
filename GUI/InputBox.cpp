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

#include "InputBox.h"
#include "../Error.h"

#include <string>

InputBox::InputBox()
{
	this->focus = 0; 
	this->index = -1;
	this->secreto = 0;
	this->maxchar = 12;
	this->parametro = 0;
	this->function = NULL;
	this->charsplus = false;
}

InputBox::InputBox(std::string title, std::string prf, int maxchars)
{
	this->titulo = title;
	this->texto = prf;
	this->focus = 0;
	this->index = -1;
	this->secreto = 0;
	this->maxchar = maxchars;
	this->parametro = 0;
	this->function = NULL;
	this->charsplus = false;
}

void InputBox::Set(int x, int y, int anc) 
{
	this->x = x;
	this->y = y;
	this->ancho = anc;
	this->alto = 14;
}

void InputBox::Borrar()
{
	if((int)texto.size() != 0) {
		texto = texto.substr(0, texto.length()-1);
	}
}

void InputBox::Escribir(char que)
{
	if((int)texto.size() >= maxchar) return;
	texto.push_back(que);
}
