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

#ifndef __H_GUI_LABEL__
#define __H_GUI_LABEL__

#include "Widget.h"
#include <string>

class Label : public Widget {
	public:
		Label(std::string title, int tam = 16);
		~Label() {}

		void Set(int x, int y);
		const char *getText() { return texto.c_str(); }
		void cambiarTexto(std::string que);
        int getTamano() { return tamano; }
	private:
		std::string texto;
        int tamano;
};
#endif
