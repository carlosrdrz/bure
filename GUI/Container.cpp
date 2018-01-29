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

#include "Container.h"

#include <list>

Container::Container() 
{
    this->boton_pulsado = false;
	this->focus = 0;
	this->index = 0;
	this->invisible = 0;
}

Container::Container(int x, int y, int ancho, int alto) {
	this->x = x;
	this->y = y;
	this->ancho = ancho;
	this->alto = alto;
	
	this->focus = 0;
	this->index = 0;
	this->invisible = 0;
    this->boton_pulsado = false;
}

Container::~Container() {
	etiq.clear();
	cajas.clear();
	pul.clear();
	images.clear();
}

void Container::Add(Label *w) {
	w->index = (int)etiq.size();
	etiq.push_back(w);
}

void Container::Add(Boton *w) {
	w->index = (int)pul.size();
	pul.push_back(w);
}

void Container::Add(InputBox *w) {
	w->index = (int)cajas.size();
	cajas.push_back(w);
	if((int)cajas.size() == 1) {
		inputboxOnFocus = cajas.begin();
		(*inputboxOnFocus)->Focus();
	}
	
}

void Container::Add(Imagen *w) {
	w->index = (int)images.size();
	images.push_back(w);
}

void Container::Add(Selector *w) {
	w->index = (int)selectores.size();
	selectores.push_back(w);
}

void Container::SetCont(int x, int y, int ancho, int alto) {
	this->x = x;
	this->y = y;
	this->ancho = ancho;
	this->alto = alto;
}

///////////////////////////////////////////////////////////
///// int elem:     	- 0: Label
/////					- 1: InputBox
/////					- 2: Boton
/////					- 3: Imagen
/////
///// int num: 			Elemento a borrar
/////////////////////////////////////////////////////////////
void Container::CloseWidget(int elem, int num) {
	if(!elem) {
		std::list<Label*>::iterator it;
		for(it = etiq.begin(); it != etiq.end(); ++it) {
			if((*it)->index == num) {
				etiq.erase(it);
				break;
			}
		}
	} else if(1 == elem) {
		std::list<InputBox*>::iterator it;
		for(it = cajas.begin(); it != cajas.end(); ++it) {
			if((*it)->index == num) {
				cajas.erase(it);
				break;
			}
		}
	} else if(2 == elem) {
		std::list<Boton*>::iterator it;
		for(it = pul.begin(); it != pul.end(); ++it) {
			if((*it)->index == num) {
				pul.erase(it);
				break;
			}
		}
	} else if(3 == elem) {
		std::list<Imagen*>::iterator it;
		for(it = images.begin(); it != images.end(); ++it) {
			if((*it)->index == num) {
				images.erase(it);
				break;
			}
		}
	}
}

void Container::changeButtonFocus(int num)
{	
	std::list<Boton*>::iterator it;
	for (it = pul.begin(); it != pul.end(); ++it) {
		if((*it)->index == num) {
			buttonOnFocus = it;
			break;
		}
	}
}

void Container::changeIBFocus(int num)
{
	if((int)cajas.size() > 0) {
		(*inputboxOnFocus)->noFocus();

		if(num >= (int)cajas.size()) {
			num -= (int)cajas.size();	
		}
	
		std::list<InputBox*>::iterator it;
		for (it = cajas.begin(); it != cajas.end(); ++it) {
			if((*it)->index == num) {
				inputboxOnFocus = it;
				break;
			}
		}
	
		(*inputboxOnFocus)->Focus();
	}

}

InputBox *Container::getIB(int ib)
{
	if(ib > (int)cajas.size()-1) return NULL;
	
	std::list<InputBox*>::iterator lb;
	for(lb = cajas.begin(); lb != cajas.end(); ++lb) {
	if((*lb)->index == ib) { 
		return (*lb);
		}
	}
	
	return NULL;
}

Label *Container::getLabel(int lb)
{
	if(lb > (int)etiq.size()-1) return NULL;
	
	std::list<Label*>::iterator eti;
	for(eti = etiq.begin(); eti != etiq.end(); ++eti) {
	if((*eti)->index == lb) {
		return (*eti);
		}
	}
	
	return NULL;
}

Boton *Container::getBoton(int b)
{
	if(b > (int)pul.size()-1) return NULL;
	
	std::list<Boton*>::iterator bu;
	for(bu = pul.begin(); bu != pul.end(); ++bu) {
	if((*bu)->index == b) { 
		return (*bu);
		}
	}
	
	return NULL;
}

Imagen *Container::getImagen(int im)
{
	if(im > (int)images.size()-1) return NULL;
	
	std::list<Imagen*>::iterator imag;
	for(imag = images.begin(); imag != images.end(); ++imag) {
	if((*imag)->index == im) { 
		return (*imag);
		}
	}

	return NULL;
}

Selector *Container::getSelector(int sel)
{
	if(sel > (int)selectores.size()-1) return NULL;
	
	std::list<Selector*>::iterator selec;
	for(selec = selectores.begin(); selec != selectores.end(); ++selec) {
		if((*selec)->index == sel) { 
			return (*selec);
		}
	}
	
	return NULL;
}
