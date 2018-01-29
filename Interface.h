#ifndef __H_INTERFACE__
#define __H_INTERFACE__

#include "GUI/Container.h"
#include "GUI/Boton.h"
#include "GUI/InputBox.h"
#include "GUI/Selector.h"

#include <list>

class Container;
class Boton;
class InputBox;
class Selector;

class Interface {
	public:
		Interface();
		~Interface();

		bool writing;
		bool bloqueado;

		void addContainer(Container *e);
		void closeContainer(int in);
		void closeContainer(Container *p);

		void changeContainerFocus(int num);
		void changeIBFocus(int num);
		void changeButtonFocus(int num);

		bool clickOnContainer(int x, int y);
		bool clickOnIB(int x, int y);
		bool clickOnButton(int x, int y);
		bool clickOnSelector(int x, int y);

		bool containerExists(Container *p);
		Container *getContainer(int cont);

		Container *getContainerClicked(int x, int y);
		InputBox *getInputBoxClicked(int x, int y);
		Boton *getButtonClicked(int x, int y);
		Selector *getSelectorClicked(int x, int y);

		Container *getContainerFocused();
		InputBox *getInputBoxFocused();
		Boton *getButtonFocused();

		// Acciones de los botones
		void ejecutarBoton(Boton *a_ejecutar);
		void ejecutarBoton(InputBox *a_ejecutar);
		void e_cerrarJuego(int param);
		void e_crearCuenta(int param);
		void e_enviarCuentaNueva(int param);
		void e_hacerLogin(int param);
		void e_pantallaNuevoPersonaje(int param);
		void e_enviarNuevoPersonaje(int param);
		void e_conectarConPersonaje(int param);
		void e_enviarMensaje(int param);

	private:
		std::list<Container*> containers;
		std::list<Container*>::iterator containerOnFocus;

		void actualizarIndex();
};

#endif
