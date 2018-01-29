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

#include <string>
#include "Label.h"

Label::Label(std::string title, int tam) 
{
	texto = title;
    tamano = tam;
}

void Label::Set(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Label::cambiarTexto(std::string que)
{
	this->texto = que;
}
