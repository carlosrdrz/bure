#pragma once

#include "widget.h"
#include "button.h"
#include "input_box.h"
#include "label.h"
#include "image.h"
#include "ui_selector.h"

#include <list>

class button;
class input_box;
class image;
class label;
class ui_selector;

class container : public widget {
public:
	int invisible;
	int index;
	bool boton_pulsado;
	
	container();
	container(int x, int y, int ancho, int alto);
	~container();

	void Add(label *w);
	void Add(input_box *w);
	void Add(button *w);
	void Add(image *w);
	void Add(ui_selector *w);

	void SetCont(int x, int y, int ancho, int alto);
	void SetInvisible() { invisible = 1; }
	void CloseWidget(int elem, int num);
	
	input_box *getIB(int ib);
	label *getLabel(int lb);
	button *getBoton(int b);
	image *getImagen(int im);
	ui_selector *getSelector(int sel);
	
	void Focus() { focus = 1; }
	void noFocus() { focus = 0; }
	int getFocus() { return focus; }
	
	void changeIBFocus(int num);
	void changeButtonFocus(int num);
	
	input_box *getIBFocused() { if((int)cajas.size() > 0) return (*inputboxOnFocus); else return NULL; }
	button *getButtonFocused() { if((int)pul.size() > 0) return (*buttonOnFocus); else return NULL; }
	
private:
	int focus;
	
	std::list<label*> etiq;
	std::list<input_box*> cajas;
	std::list<button*> pul;
	std::list<image*> images;
	std::list<ui_selector*> selectores;
	
	std::list<input_box*>::iterator inputboxOnFocus;
	std::list<button*>::iterator buttonOnFocus;
};
