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

#include "container.h"

#include <list>

container::container()
{
    this->boton_pulsado = false;
	this->focus = 0;
	this->index = 0;
	this->invisible = 0;
}

container::container(int x, int y, int ancho, int alto) {
	this->x = x;
	this->y = y;
	this->width = ancho;
	this->height = alto;
	
	this->focus = 0;
	this->index = 0;
	this->invisible = 0;
    this->boton_pulsado = false;
}

container::~container() {
	etiq.clear();
	cajas.clear();
	pul.clear();
	images.clear();
}

void container::Add(label *w) {
	w->index = (int)etiq.size();
	etiq.push_back(w);
}

void container::Add(button *w) {
	w->index = (int)pul.size();
	pul.push_back(w);
}

void container::Add(input_box *w) {
	w->index = (int)cajas.size();
	cajas.push_back(w);
	if((int)cajas.size() == 1) {
		inputboxOnFocus = cajas.begin();
		(*inputboxOnFocus)->Focus();
	}
	
}

void container::Add(image *w) {
	w->index = (int)images.size();
	images.push_back(w);
}

void container::Add(ui_selector *w) {
	w->index = (int)selectores.size();
	selectores.push_back(w);
}

void container::SetCont(int x, int y, int ancho, int alto) {
	this->x = x;
	this->y = y;
	this->width = ancho;
	this->height = alto;
}

///////////////////////////////////////////////////////////
///// int elem:     	- 0: Label
/////					- 1: InputBox
/////					- 2: Boton
/////					- 3: Imagen
/////
///// int num: 			Elemento a borrar
/////////////////////////////////////////////////////////////
void container::CloseWidget(int elem, int num) {
	if(!elem) {
		std::list<label*>::iterator it;
		for(it = etiq.begin(); it != etiq.end(); ++it) {
			if((*it)->index == num) {
				etiq.erase(it);
				break;
			}
		}
	} else if(1 == elem) {
		std::list<input_box*>::iterator it;
		for(it = cajas.begin(); it != cajas.end(); ++it) {
			if((*it)->index == num) {
				cajas.erase(it);
				break;
			}
		}
	} else if(2 == elem) {
		std::list<button*>::iterator it;
		for(it = pul.begin(); it != pul.end(); ++it) {
			if((*it)->index == num) {
				pul.erase(it);
				break;
			}
		}
	} else if(3 == elem) {
		std::list<image*>::iterator it;
		for(it = images.begin(); it != images.end(); ++it) {
			if((*it)->index == num) {
				images.erase(it);
				break;
			}
		}
	}
}

void container::changeButtonFocus(int num)
{	
	std::list<button*>::iterator it;
	for (it = pul.begin(); it != pul.end(); ++it) {
		if((*it)->index == num) {
			buttonOnFocus = it;
			break;
		}
	}
}

void container::changeIBFocus(int num)
{
	if((int)cajas.size() > 0) {
		(*inputboxOnFocus)->noFocus();

		if(num >= (int)cajas.size()) {
			num -= (int)cajas.size();	
		}
	
		std::list<input_box*>::iterator it;
		for (it = cajas.begin(); it != cajas.end(); ++it) {
			if((*it)->index == num) {
				inputboxOnFocus = it;
				break;
			}
		}
	
		(*inputboxOnFocus)->Focus();
	}

}

input_box *container::getIB(int ib)
{
	if(ib > (int)cajas.size()-1) return NULL;
	
	std::list<input_box*>::iterator lb;
	for(lb = cajas.begin(); lb != cajas.end(); ++lb) {
	if((*lb)->index == ib) { 
		return (*lb);
		}
	}
	
	return NULL;
}

label *container::getLabel(int lb)
{
	if(lb > (int)etiq.size()-1) return NULL;
	
	std::list<label*>::iterator eti;
	for(eti = etiq.begin(); eti != etiq.end(); ++eti) {
	if((*eti)->index == lb) {
		return (*eti);
		}
	}
	
	return NULL;
}

button *container::getBoton(int b)
{
	if(b > (int)pul.size()-1) return NULL;
	
	std::list<button*>::iterator bu;
	for(bu = pul.begin(); bu != pul.end(); ++bu) {
	if((*bu)->index == b) { 
		return (*bu);
		}
	}
	
	return NULL;
}

image *container::getImagen(int im)
{
	if(im > (int)images.size()-1) return NULL;
	
	std::list<image*>::iterator imag;
	for(imag = images.begin(); imag != images.end(); ++imag) {
	if((*imag)->index == im) { 
		return (*imag);
		}
	}

	return NULL;
}

ui_selector *container::getSelector(int sel)
{
	if(sel > (int)selectores.size()-1) return NULL;
	
	std::list<ui_selector*>::iterator selec;
	for(selec = selectores.begin(); selec != selectores.end(); ++selec) {
		if((*selec)->index == sel) { 
			return (*selec);
		}
	}
	
	return NULL;
}
