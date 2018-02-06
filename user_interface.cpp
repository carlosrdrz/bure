#include "SDL.h"
#include "error.h"
#include "GUI/container.h"
#include "game.h"
#include "network.h"
#include "graphics.h"
#include "config.h"

#include <sstream>

#define ACTUAL_VERSION "0.2.0"

typedef void (user_interface::*InterfaceFunc)(int param);

extern game *gameInstance;
extern network *net;
extern graphics *pantalla;
extern config *configInstance;

user_interface::user_interface() {
	// EMPEZAMOS CREANDO EL MENU PRINCIPAL
	// Crear el contenedor principal
	container *men = new container(362, 309, 300, 185);

	// Etiquetas de texto
	label *iniciar_sesion = new label("INICIA SESION");
		iniciar_sesion->Set(75, 25);
	label *guardaruser = new label("RECORDAR USUARIO", 8);
		guardaruser->Set(35, 140);
	label *guardarpass = new label("RECORDAR PASSWORD", 8);
		guardarpass->Set(35, 160);

	ui_selector *seled = new ui_selector(220, 137);
		if(configInstance->getBoolValueOf("user")) {
			seled->addOption("SI");
			seled->addOption("NO");
		} else {
			seled->addOption("NO");
			seled->addOption("SI");
		}
	ui_selector *seled2 = new ui_selector(220, 157);
		if(configInstance->getBoolValueOf("pass")) {
			seled2->addOption("SI");
			seled2->addOption("NO");
		} else {
			seled2->addOption("NO");
			seled2->addOption("SI");
		}

	// InputBoxes
	input_box *user;
	if(configInstance->getBoolValueOf("user")) {
		user = new input_box("USUARIO:", configInstance->getValueOf("user"), 12);
	} else {
		user = new input_box("USUARIO:", "", 12);
	}

	user->Set(120, 55, 101);
	user->function = &user_interface::e_hacerLogin;

	input_box *pass;
	if(configInstance->getBoolValueOf("pass")) {
		pass = new input_box("PASSWORD:", configInstance->getValueOf("pass"), 12);
	} else {
		pass = new input_box("PASSWORD:", "", 12);
	}

	pass->Set(120, 80, 101);
	pass->EsSecreto();
	pass->function = &user_interface::e_hacerLogin;

	// Botones
	button *enviar = new button("ENTRAR");
		enviar->Set(25, 110, 70, 20);
		enviar->function = &user_interface::e_hacerLogin;
	button *regist = new button("NUEVA CUENTA");
		regist->Set(100, 110, 100, 20);
		regist->function = &user_interface::e_crearCuenta;
	button *salir = new button("SALIR");
		salir->Set(205, 110, 70, 20);
		salir->function = &user_interface::e_cerrarJuego;

	men->Add(iniciar_sesion); // Añadimos labels
	men->Add(guardaruser);
	men->Add(guardarpass);
	men->Add(user); // Añadimos inputs
	men->Add(pass);
	men->Add(enviar); // Añadimos botones
	men->Add(regist);
	men->Add(salir);
	men->Add(seled); // Añadir selector
	men->Add(seled2);

    writing = true;
    blocked = false;

	this->addContainer(men); // Añadir el container a la UI
	containerOnFocus = containers.begin();
	(*containerOnFocus)->Focus();
}

user_interface::~user_interface() {
	containers.clear();
}

void user_interface::addContainer(container *e) {
	e->index = (int)containers.size();
	containers.push_back(e);
    if((int)containers.size() == 1) changeContainerFocus(0);
}

void user_interface::closeContainer(int in) {
	std::list<container*>::iterator it;

	for (it = containers.begin(); it != containers.end(); ++it) {
		if((*it)->index == in) {
		    if((*it)->index == getContainerFocused()->index) {
		        changeContainerFocus(getContainerFocused()->index-1);
		    }

			containers.erase(it);
			break;
		}
	}
	this->actualizarIndex();
}

void user_interface::closeContainer(container *p) {
	if(NULL != p) {
		std::list<container*>::iterator it;

		for (it = containers.begin(); it != containers.end(); ++it) {
			if((*it) == p) {
				if((*it) == getContainerFocused()) {
				    changeContainerFocus(getContainerFocused()->index-1);
				}

				containers.erase(it);
				break;
			}
		}
		this->actualizarIndex();
	}
}

void user_interface::actualizarIndex() {
	std::list<container*>::iterator it;
	int in = 0;
	for (it = containers.begin(); it != containers.end(); ++it) {
		(*it)->index = in;
		in++;
	}

	if((int)containers.size() == 1) {
		containerOnFocus = containers.begin();
		(*containerOnFocus)->Focus();
	}
}

container *user_interface::getContainer(int cont) {
	std::list<container*>::iterator it;

    if(cont > (int)containers.size()-1) return NULL;

	for(it = containers.begin(); it != containers.end(); ++it) {
		if((*it)->index == cont) {
			return (*it);
		}
	}

	return NULL;
}

bool user_interface::containerExists(container *p) {
	bool res = false;
	std::list<container*>::iterator it;

    if(NULL != p) {
		for(it = containers.begin(); it != containers.end(); ++it) {
			if((*it) == p) {
				res = true;
			}
		}
	}

	return res;
}

void user_interface::changeContainerFocus(int num)
{
    if((int)containers.size() > 1) {
    	(*containerOnFocus)->noFocus();
    }

	std::list<container*>::iterator it;
	for (it = containers.begin(); it != containers.end(); ++it) {
		if((*it)->index == num) {
			containerOnFocus = it;
			break;
		}
	}

	(*containerOnFocus)->Focus();
}

void user_interface::changeIBFocus(int num)
{
	(*containerOnFocus)->changeIBFocus(num);
}

void user_interface::changeButtonFocus(int num)
{
	(*containerOnFocus)->changeButtonFocus(num);
}

container *user_interface::getContainerFocused()
{
	if((int)containers.size() == 0) return NULL;
	return (*containerOnFocus);
}

input_box *user_interface::getInputBoxFocused()
{
	if((int)containers.size() == 0) return NULL;
	return (*containerOnFocus)->getIBFocused();
}

button *user_interface::getButtonFocused()
{
	return (*containerOnFocus)->getButtonFocused();
}

bool user_interface::clickOnContainer(int x, int y)
{
	if((int)containers.size() > 0) {
		std::list<container*>::iterator itib;
		for(itib = containers.begin(); itib != containers.end(); ++itib) {
			if(x > (*itib)->x && x < ((*itib)->x + (*itib)->width) && y > (*itib)->y
			&& y < ((*itib)->y + (*itib)->height)) {
				return true;
			}
		}
	}
	return false;
}

bool user_interface::clickOnIB(int x, int y)
{
	int z;
	if((int)containers.size() > 0) {
		for(z = 0; getContainerFocused()->getIB(z) != NULL; ++z) {
			if(x > (getContainerFocused()->x + getContainerFocused()->getIB(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getIB(z)->x + getContainerFocused()->getIB(z)->width) && y > (getContainerFocused()->y
			+ getContainerFocused()->getIB(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getIB(z)->y
			+ getContainerFocused()->getIB(z)->height))
			{
				return true;
			}
		}
	}
	return false;
}

bool user_interface::clickOnButton(int x, int y)
{
	int z;
	if((int)containers.size() > 0) {
		for(z = 0; getContainerFocused()->getBoton(z) != NULL; ++z) {
			if(x > (getContainerFocused()->x + getContainerFocused()->getBoton(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getBoton(z)->x + getContainerFocused()->getBoton(z)->width)&& y > (getContainerFocused()->y
			+ getContainerFocused()->getBoton(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getBoton(z)->y
			+ getContainerFocused()->getBoton(z)->height))
			{
				return true;
			}
		}
	}
	return false;
}

bool user_interface::clickOnSelector(int x, int y)
{
	int z;
	if(containers.size() > 0) {
		for(z = 0; getContainerFocused()->getSelector(z) != NULL; ++z) {
			if(( x > (getContainerFocused()->x + getContainerFocused()->getSelector(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + 9) && y > (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y)
			&& y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) || ( x > (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 17) && x < (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 26) &&  y > (getContainerFocused()->y
			+ getContainerFocused()->getSelector(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) ) {
				return true;
			}
			/*

			*/
		}
	}
	return false;
}

container *user_interface::getContainerClicked(int x, int y)
{
	std::list<container*>::iterator itib, toreturn;
	toreturn = containers.end();
	for(itib = containers.begin(); itib != containers.end(); ++itib) {
		if(x > (*itib)->x && x < ((*itib)->x + (*itib)->width) && y > (*itib)->y
		&& y < ((*itib)->y + (*itib)->height)) {
			toreturn = itib;
		}
	}

	if(toreturn == containers.end()) return NULL;

	return (*toreturn);
}

input_box *user_interface::getInputBoxClicked(int x, int y)
{
	int z;
	input_box *toreturn = NULL;
	for(z = 0; getContainerFocused()->getIB(z) != NULL; ++z) {
		if(x > (getContainerFocused()->x + getContainerFocused()->getIB(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getIB(z)->x + getContainerFocused()->getIB(z)->width)&& y > (getContainerFocused()->y
		+ getContainerFocused()->getIB(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getIB(z)->y
		+ getContainerFocused()->getIB(z)->height))
		{
			toreturn = getContainerFocused()->getIB(z);
		}
	}
	return toreturn;
}

button *user_interface::getButtonClicked(int x, int y)
{
	int z;
	button *toreturn = NULL;
	for(z = 0; getContainerFocused()->getBoton(z) != NULL; ++z) {
		if(x > (getContainerFocused()->x + getContainerFocused()->getBoton(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getBoton(z)->x + getContainerFocused()->getBoton(z)->width)&& y > (getContainerFocused()->y
		+ getContainerFocused()->getBoton(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getBoton(z)->y
		+ getContainerFocused()->getBoton(z)->height))
		{
			toreturn = getContainerFocused()->getBoton(z);
		}
	}
	return toreturn;
}

ui_selector *user_interface::getSelectorClicked(int x, int y)
{
	int z;
	ui_selector *toreturn = NULL;
	for(z = 0; getContainerFocused()->getSelector(z) != NULL; ++z) {
		if(( x > (getContainerFocused()->x + getContainerFocused()->getSelector(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + 9) && y > (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y)
		&& y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) || ( x > (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 17) && x < (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->width + 26) &&  y > (getContainerFocused()->y
		+ getContainerFocused()->getSelector(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) ) {
			toreturn = getContainerFocused()->getSelector(z);
		}
	}
	return toreturn;
}

//////////////////////////
// ACCIONES DE LOS BOTONES
//////////////////////////
void user_interface::ejecutarBoton(button *a_ejecutar)
{
    InterfaceFunc ejec;
    ejec = a_ejecutar->function;
    (this->*ejec)(a_ejecutar->parametro);
}

void user_interface::ejecutarBoton(input_box *a_ejecutar)
{
    InterfaceFunc ejec;
    ejec = a_ejecutar->function;
    (this->*ejec)(a_ejecutar->parametro);
}


void user_interface::e_cerrarJuego(int param)
{
	if(gameInstance->logged) net->sendPacket("0_0_NOTHING");
	SDL_Delay(250);
    gameInstance->cerrar();
}

void user_interface::e_crearCuenta(int param)
{
	static container *p = NULL;

	if(this->containerExists(p)) {
		this->closeContainer(p);
	} else {
		container *cn = new container;
			cn->SetCont(395, 520, 230, 120);
		input_box *b = new input_box("Usuario:","",12);
			b->Set(90, 20, 101);
			b->function = &user_interface::e_enviarCuentaNueva;
		input_box *bp = new input_box("Password:","",12);
			bp->Set(90, 50, 101);
			bp->function = &user_interface::e_enviarCuentaNueva;
		button *bo = new button("Crear cuenta");
			bo->Set(20, 80, 90, 20);
			bo->function = &user_interface::e_enviarCuentaNueva;
		button *ca = new button("Cancelar");
			ca->Set(120, 80, 90, 20);
			ca->function = &user_interface::e_crearCuenta;

		cn->Add(b);
		cn->Add(bp);
		cn->Add(bo);
		cn->Add(ca);

		this->addContainer(cn);
		this->changeContainerFocus(cn->index);
		p = cn;
	}
}

void user_interface::e_enviarCuentaNueva(int param)
{

}

void user_interface::e_hacerLogin(int param)
{
	this->blocked = true;

	if(this->getContainer(1) == NULL) {
	container *cn = new container(260, 170, 500, 50);
	label *a = new label("CONECTANDO CON EL SERVIDOR...", 16);
		a->Set(19, 19);
		cn->Add(a);
	this->addContainer(cn);

	}

	net->connectToTheServer();
	std::string bf = "1_0_" + this->getContainer(0)->getIB(0)->texto + "_" + this->getContainer(0)->getIB(1)->texto + "_" + ACTUAL_VERSION;
	net->sendPacket(bf);

	this->getContainer(1)->getLabel(0)->cambiarTexto("VERIFICANDO DATOS...");

	// Comprobar y guardar la configuración
	if(!this->getContainer(0)->getSelector(0)->getSelected().compare("SI")) {
		configInstance->setValueOf("user",  this->getContainer(0)->getIB(0)->texto);
	} else {
		configInstance->setValueOf("user",  "0");
	}
	if(!this->getContainer(0)->getSelector(1)->getSelected().compare("SI")) {
		configInstance->setValueOf("pass",  this->getContainer(0)->getIB(1)->texto);
	} else {
		configInstance->setValueOf("pass",  "0");
	}
}

void user_interface::e_pantallaNuevoPersonaje(int param)
{
	static container *p = NULL;

	if(this->containerExists(p)) {
		this->closeContainer(p);
	} else {
		if(this->getContainer(2) != NULL) {
			container *cn = new container;
				cn->SetCont(310, 270, 400, 250);
			label *l = new label("CREAR NUEVO PERSONAJE", 16);
				l->Set(60, 20);
			label *elige = new label("Elige un estilo de personaje", 8);
				elige->Set(30, 95);
			input_box *b = new input_box("Nombre:","",12);
				b->Set(80, 50, 101);
				b->function = &user_interface::e_enviarNuevoPersonaje;
			button *bo = new button("Nacer");
				bo->Set(20, 210, 90, 20);
				bo->function = &user_interface::e_enviarNuevoPersonaje;
			button *ca = new button("Cancelar");
				ca->Set(120, 210, 90, 20);
				ca->function = &user_interface::e_pantallaNuevoPersonaje;
			ui_selector *seled = new ui_selector(30, 110);
				seled->addOption("1"); seled->addOption("2"); seled->addOption("3");
				seled->addOption("4"); seled->addOption("5"); seled->addOption("6");
				seled->addOption("7"); seled->addOption("8"); seled->addOption("9");
				seled->addOption("10"); seled->addOption("11");	seled->addOption("12");
				seled->addOption("13"); seled->addOption("14");	seled->addOption("15");
				seled->addOption("16");
			image *n = new image();
				n->set(230, 50);
				n->setDinamic();
				n->setDinamicSelector(0, "data/chars_avatar/", ".png");

			cn->Add(l);
			cn->Add(elige);
			cn->Add(b);
			cn->Add(n);
			cn->Add(bo);
			cn->Add(ca);
			cn->Add(seled);

			this->addContainer(cn);
			this->changeContainerFocus(cn->index);
		}
	}
}

void user_interface::e_enviarNuevoPersonaje(int param)
{
	std::string bff = "1_2_" + this->getContainer(3)->getIB(0)->texto + "_" + this->getContainer(3)->getSelector(0)->getSelected();
    net->sendPacket(bff);

    label *l = new label("CREANDO PERSONAJE...", 8);
     	l->Set(50, 120);

	this->getContainer(3)->Add(l);
}

void user_interface::e_conectarConPersonaje(int param)
{
	this->closeContainer(2);
	this->closeContainer(1);
	this->getContainer(0)->y += 150;
	this->getContainer(0)->getLabel(0)->cambiarTexto("CONECTANDO AL MUNDO...");

	std::string bff("1_3_");
	std::stringstream aux;
	aux << param;
	bff.append(aux.str());
	net->sendPacket(bff);
}

void user_interface::e_enviarMensaje(int param)
{
	std::string bff = "3_0_" + this->getContainer(0)->getIB(0)->texto;
	net->sendPacket(bff);

    this->closeContainer(0);
    this->writing = false;
    // SDL_EnableKeyRepeat(100, 30);
}
