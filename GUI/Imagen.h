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

#ifndef __H_GUI_IMAGEN__
#define __H_GUI_IMAGEN__

#include "SDL_image.h"
#include "Widget.h"
#include <string>

class Imagen : public Widget {
public:
	SDL_Surface *img;
	int selector;
	std::string currentpath;
	std::string inicio;
	std::string final;

	Imagen() { 
		img = NULL;
		dinamic = false;
	}
	Imagen(std::string archivo) {
		img = IMG_Load(archivo.c_str());
		dinamic = false;
	}
	~Imagen() { SDL_FreeSurface(img); }

	void Set(int x, int y) {
		this->x = x;
		this->y = y;
	}
	
	void setDinamic() { dinamic = true; }
	void setDinamicSelector(int sel, std::string init, std::string fin) {
		selector = sel;
		inicio = init;
		final = fin;
	}
	
	bool isDinamic() { return dinamic; }

private:
	bool dinamic;
};
#endif
