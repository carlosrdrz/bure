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

#ifndef __H_GUI_CONTAINER__
#define __H_GUI_CONTAINER__
#include "Widget.h"
#include "Boton.h"
#include "InputBox.h"
#include "Label.h"
#include "Imagen.h"
#include "Selector.h"

#include <list>

class Boton;
class InputBox;
class Imagen;
class Label;
class Selector;

class Container : public Widget {
public:
	int invisible;
	int index;
	bool boton_pulsado;
	
	Container();
	Container(int x, int y, int ancho, int alto);
	~Container();

	void Add(Label *w);
	void Add(InputBox *w);
	void Add(Boton *w);
	void Add(Imagen *w);
	void Add(Selector *w);
	
	void SetCont(int x, int y, int ancho, int alto);
	void SetInvisible() { invisible = 1; }
	void CloseWidget(int elem, int num);
	
	InputBox *getIB(int ib);
	Label *getLabel(int lb);
	Boton *getBoton(int b);
	Imagen *getImagen(int im);
	Selector *getSelector(int sel);
	
	void Focus() { focus = 1; }
	void noFocus() { focus = 0; }
	int getFocus() { return focus; }
	
	void changeIBFocus(int num);
	void changeButtonFocus(int num);
	
	InputBox *getIBFocused() { if((int)cajas.size() > 0) return (*inputboxOnFocus); else return NULL; }
	Boton *getButtonFocused() { if((int)pul.size() > 0) return (*buttonOnFocus); else return NULL; }
	
private:
	int focus;
	
	std::list<Label*> etiq;
	std::list<InputBox*> cajas;
	std::list<Boton*> pul;
	std::list<Imagen*> images;
	std::list<Selector*> selectores;
	
	std::list<InputBox*>::iterator inputboxOnFocus;
	std::list<Boton*>::iterator buttonOnFocus;
};
#endif
