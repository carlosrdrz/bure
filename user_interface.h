#ifndef __H_INTERFACE__
#define __H_INTERFACE__

#include "GUI/container.h"
#include "GUI/button.h"
#include "GUI/input_box.h"
#include "GUI/ui_selector.h"

#include <list>

class container;
class button;
class input_box;
class ui_selector;

class user_interface {
	public:
		user_interface();
		~user_interface();

		bool writing;
		bool blocked;

		void addContainer(container *e);
		void closeContainer(int in);
		void closeContainer(container *p);

		void changeContainerFocus(int num);
		void changeIBFocus(int num);
		void changeButtonFocus(int num);

		bool clickOnContainer(int x, int y);
		bool clickOnIB(int x, int y);
		bool clickOnButton(int x, int y);
		bool clickOnSelector(int x, int y);

		bool containerExists(container *p);
		container *getContainer(int cont);

		container *getContainerClicked(int x, int y);
		input_box *getInputBoxClicked(int x, int y);
		button *getButtonClicked(int x, int y);
		ui_selector *getSelectorClicked(int x, int y);

		container *getContainerFocused();
		input_box *getInputBoxFocused();
		button *getButtonFocused();

		// Acciones de los botones
		void ejecutarBoton(button *a_ejecutar);
		void ejecutarBoton(input_box *a_ejecutar);
		void e_cerrarJuego(int param);
		void e_loadMap(int param);

	private:
		std::list<container*> containers;
		std::list<container*>::iterator containerOnFocus;

		void actualizarIndex();
};

#endif
