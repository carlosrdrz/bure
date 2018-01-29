#include "SDL.h"
#include "Error.h"
#include "GUI/Container.h"
#include "GUI/Boton.h"
#include "GUI/InputBox.h"
#include "GUI/Selector.h"
#include "Interface.h"
#include "Juego.h"
#include "Network.h"
#include "Graficos.h"
#include "Config.h"

#include <list>
#include <sstream>

#define ACTUAL_VERSION "0.2.0"

typedef void (Interface::*InterfaceFunc)(int param);

extern Juego *game;
extern Network *net;
extern Graficos *pantalla;
extern Config *cfg;

Interface::Interface() {
	// EMPEZAMOS CREANDO EL MENU PRINCIPAL
	// Crear el contenedor principal
	Container *men = new Container(362, 309, 300, 185);

	// Etiquetas de texto
	Label *iniciar_sesion = new Label("INICIA SESION");
		iniciar_sesion->Set(75, 25);
	Label *guardaruser = new Label("RECORDAR USUARIO", 8);
		guardaruser->Set(35, 140);
	Label *guardarpass = new Label("RECORDAR PASSWORD", 8);
		guardarpass->Set(35, 160);

	Selector *seled = new Selector(220, 137);
		if(cfg->getBoolValueOf("user")) {
			seled->addOption("SI");
			seled->addOption("NO");
		} else {
			seled->addOption("NO");
			seled->addOption("SI");
		}
	Selector *seled2 = new Selector(220, 157);
		if(cfg->getBoolValueOf("pass")) {
			seled2->addOption("SI");
			seled2->addOption("NO");
		} else {
			seled2->addOption("NO");
			seled2->addOption("SI");
		}

	// InputBoxes
	InputBox *user;
	if(cfg->getBoolValueOf("user")) {
		user = new InputBox("USUARIO:", cfg->getValueOf("user"), 12);
	} else {
		user = new InputBox("USUARIO:", "", 12);
	}

	user->Set(120, 55, 101);
	user->function = &Interface::e_hacerLogin;

	InputBox *pass;
	if(cfg->getBoolValueOf("pass")) {
		pass = new InputBox("PASSWORD:", cfg->getValueOf("pass"), 12);
	} else {
		pass = new InputBox("PASSWORD:", "", 12);
	}

	pass->Set(120, 80, 101);
	pass->EsSecreto();
	pass->function = &Interface::e_hacerLogin;

	// Botones
	Boton *enviar = new Boton("ENTRAR");
		enviar->Set(25, 110, 70, 20);
		enviar->function = &Interface::e_hacerLogin;
	Boton *regist = new Boton("NUEVA CUENTA");
		regist->Set(100, 110, 100, 20);
		regist->function = &Interface::e_crearCuenta;
	Boton *salir = new Boton("SALIR");
		salir->Set(205, 110, 70, 20);
		salir->function = &Interface::e_cerrarJuego;

	men->Add(iniciar_sesion); // Añadimos labels
	men ->Add(guardaruser);
	men ->Add(guardarpass);
	men->Add(user); // Añadimos inputs
	men->Add(pass);
	men->Add(enviar); // Añadimos botones
	men->Add(regist);
	men->Add(salir);
	men->Add(seled); // Añadir selector
	men ->Add(seled2);

    writing = true;
    bloqueado = false;

	this->addContainer(men); // Añadir el container a la UI
	containerOnFocus = containers.begin();
	(*containerOnFocus)->Focus();
}

Interface::~Interface() {
	containers.clear();
}

void Interface::addContainer(Container *e) {
	e->index = (int)containers.size();
	containers.push_back(e);
    if((int)containers.size() == 1) changeContainerFocus(0);
}

void Interface::closeContainer(int in) {
	std::list<Container*>::iterator it;

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

void Interface::closeContainer(Container *p) {
	if(NULL != p) {
		std::list<Container*>::iterator it;

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

void Interface::actualizarIndex() {
	std::list<Container*>::iterator it;
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

Container *Interface::getContainer(int cont) {
	std::list<Container*>::iterator it;

    if(cont > (int)containers.size()-1) return NULL;

	for(it = containers.begin(); it != containers.end(); ++it) {
		if((*it)->index == cont) {
			return (*it);
		}
	}

	return NULL;
}

bool Interface::containerExists(Container *p) {
	bool res = false;
	std::list<Container*>::iterator it;

    if(NULL != p) {
		for(it = containers.begin(); it != containers.end(); ++it) {
			if((*it) == p) {
				res = true;
			}
		}
	}

	return res;
}

void Interface::changeContainerFocus(int num)
{
    if((int)containers.size() > 1) {
    	(*containerOnFocus)->noFocus();
    }

	std::list<Container*>::iterator it;
	for (it = containers.begin(); it != containers.end(); ++it) {
		if((*it)->index == num) {
			containerOnFocus = it;
			break;
		}
	}

	(*containerOnFocus)->Focus();
}

void Interface::changeIBFocus(int num)
{
	(*containerOnFocus)->changeIBFocus(num);
}

void Interface::changeButtonFocus(int num)
{
	(*containerOnFocus)->changeButtonFocus(num);
}

Container *Interface::getContainerFocused()
{
	if((int)containers.size() == 0) return NULL;
	return (*containerOnFocus);
}

InputBox *Interface::getInputBoxFocused()
{
	if((int)containers.size() == 0) return NULL;
	return (*containerOnFocus)->getIBFocused();
}

Boton *Interface::getButtonFocused()
{
	return (*containerOnFocus)->getButtonFocused();
}

bool Interface::clickOnContainer(int x, int y)
{
	if((int)containers.size() > 0) {
		std::list<Container*>::iterator itib;
		for(itib = containers.begin(); itib != containers.end(); ++itib) {
			if(x > (*itib)->x && x < ((*itib)->x + (*itib)->ancho) && y > (*itib)->y
			&& y < ((*itib)->y + (*itib)->alto)) {
				return true;
			}
		}
	}
	return false;
}

bool Interface::clickOnIB(int x, int y)
{
	int z;
	if((int)containers.size() > 0) {
		for(z = 0; getContainerFocused()->getIB(z) != NULL; ++z) {
			if(x > (getContainerFocused()->x + getContainerFocused()->getIB(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getIB(z)->x + getContainerFocused()->getIB(z)->ancho) && y > (getContainerFocused()->y
			+ getContainerFocused()->getIB(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getIB(z)->y
			+ getContainerFocused()->getIB(z)->alto))
			{
				return true;
			}
		}
	}
	return false;
}

bool Interface::clickOnButton(int x, int y)
{
	int z;
	if((int)containers.size() > 0) {
		for(z = 0; getContainerFocused()->getBoton(z) != NULL; ++z) {
			if(x > (getContainerFocused()->x + getContainerFocused()->getBoton(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getBoton(z)->x + getContainerFocused()->getBoton(z)->ancho)&& y > (getContainerFocused()->y
			+ getContainerFocused()->getBoton(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getBoton(z)->y
			+ getContainerFocused()->getBoton(z)->alto))
			{
				return true;
			}
		}
	}
	return false;
}

bool Interface::clickOnSelector(int x, int y)
{
	int z;
	if(containers.size() > 0) {
		for(z = 0; getContainerFocused()->getSelector(z) != NULL; ++z) {
			if(( x > (getContainerFocused()->x + getContainerFocused()->getSelector(z)->x) && x < (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + 9) && y > (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y)
			&& y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) || ( x > (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->ancho + 17) && x < (getContainerFocused()->x
			+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->ancho + 26) &&  y > (getContainerFocused()->y
			+ getContainerFocused()->getSelector(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) ) {
				return true;
			}
			/*

			*/
		}
	}
	return false;
}

Container *Interface::getContainerClicked(int x, int y)
{
	std::list<Container*>::iterator itib, toreturn;
	toreturn = containers.end();
	for(itib = containers.begin(); itib != containers.end(); ++itib) {
		if(x > (*itib)->x && x < ((*itib)->x + (*itib)->ancho) && y > (*itib)->y
		&& y < ((*itib)->y + (*itib)->alto)) {
			toreturn = itib;
		}
	}

	if(toreturn == containers.end()) return NULL;

	return (*toreturn);
}

InputBox *Interface::getInputBoxClicked(int x, int y)
{
	int z;
	InputBox *toreturn = NULL;
	for(z = 0; getContainerFocused()->getIB(z) != NULL; ++z) {
		if(x > (getContainerFocused()->x + getContainerFocused()->getIB(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getIB(z)->x + getContainerFocused()->getIB(z)->ancho)&& y > (getContainerFocused()->y
		+ getContainerFocused()->getIB(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getIB(z)->y
		+ getContainerFocused()->getIB(z)->alto))
		{
			toreturn = getContainerFocused()->getIB(z);
		}
	}
	return toreturn;
}

Boton *Interface::getButtonClicked(int x, int y)
{
	int z;
	Boton *toreturn = NULL;
	for(z = 0; getContainerFocused()->getBoton(z) != NULL; ++z) {
		if(x > (getContainerFocused()->x + getContainerFocused()->getBoton(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getBoton(z)->x + getContainerFocused()->getBoton(z)->ancho)&& y > (getContainerFocused()->y
		+ getContainerFocused()->getBoton(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getBoton(z)->y
		+ getContainerFocused()->getBoton(z)->alto))
		{
			toreturn = getContainerFocused()->getBoton(z);
		}
	}
	return toreturn;
}

Selector *Interface::getSelectorClicked(int x, int y)
{
	int z;
	Selector *toreturn = NULL;
	for(z = 0; getContainerFocused()->getSelector(z) != NULL; ++z) {
		if(( x > (getContainerFocused()->x + getContainerFocused()->getSelector(z)->x) && x < (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + 9) && y > (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y)
		&& y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) || ( x > (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->ancho + 17) && x < (getContainerFocused()->x
		+ getContainerFocused()->getSelector(z)->x + getContainerFocused()->getSelector(z)->ancho + 26) &&  y > (getContainerFocused()->y
		+ getContainerFocused()->getSelector(z)->y) && y < (getContainerFocused()->y + getContainerFocused()->getSelector(z)->y + 14)) ) {
			toreturn = getContainerFocused()->getSelector(z);
		}
	}
	return toreturn;
}

//////////////////////////
// ACCIONES DE LOS BOTONES
//////////////////////////
void Interface::ejecutarBoton(Boton *a_ejecutar)
{
    InterfaceFunc ejec;
    ejec = a_ejecutar->function;
    (this->*ejec)(a_ejecutar->parametro);
}

void Interface::ejecutarBoton(InputBox *a_ejecutar)
{
    InterfaceFunc ejec;
    ejec = a_ejecutar->function;
    (this->*ejec)(a_ejecutar->parametro);
}


void Interface::e_cerrarJuego(int param)
{
	if(game->logged) net->sendPacket("0_0_NOTHING");
	SDL_Delay(250);
    game->cerrar();
}

void Interface::e_crearCuenta(int param)
{
	static Container *p = NULL;

	if(this->containerExists(p)) {
		this->closeContainer(p);
	} else {
		Container *cn = new Container;
			cn->SetCont(395, 520, 230, 120);
		InputBox *b = new InputBox("Usuario:","",12);
			b->Set(90, 20, 101);
			b->function = &Interface::e_enviarCuentaNueva;
		InputBox *bp = new InputBox("Password:","",12);
			bp->Set(90, 50, 101);
			bp->function = &Interface::e_enviarCuentaNueva;
		Boton *bo = new Boton("Crear cuenta");
			bo->Set(20, 80, 90, 20);
			bo->function = &Interface::e_enviarCuentaNueva;
		Boton *ca = new Boton("Cancelar");
			ca->Set(120, 80, 90, 20);
			ca->function = &Interface::e_crearCuenta;

		cn->Add(b);
		cn->Add(bp);
		cn->Add(bo);
		cn->Add(ca);

		this->addContainer(cn);
		this->changeContainerFocus(cn->index);
		p = cn;
	}
}

void Interface::e_enviarCuentaNueva(int param)
{

}

void Interface::e_hacerLogin(int param)
{
	this->bloqueado = true;

	if(this->getContainer(1) == NULL) {
	Container *cn = new Container(260, 170, 500, 50);
	Label *a = new Label("CONECTANDO CON EL SERVIDOR...", 16);
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
		cfg->setValueOf("user",  this->getContainer(0)->getIB(0)->texto);
	} else {
		cfg->setValueOf("user",  "0");
	}
	if(!this->getContainer(0)->getSelector(1)->getSelected().compare("SI")) {
		cfg->setValueOf("pass",  this->getContainer(0)->getIB(1)->texto);
	} else {
		cfg->setValueOf("pass",  "0");
	}
}

void Interface::e_pantallaNuevoPersonaje(int param)
{
	static Container *p = NULL;

	if(this->containerExists(p)) {
		this->closeContainer(p);
	} else {
		if(this->getContainer(2) != NULL) {
			Container *cn = new Container;
				cn->SetCont(310, 270, 400, 250);
			Label *l = new Label("CREAR NUEVO PERSONAJE", 16);
				l->Set(60, 20);
			Label *elige = new Label("Elige un estilo de personaje", 8);
				elige->Set(30, 95);
			InputBox *b = new InputBox("Nombre:","",12);
				b->Set(80, 50, 101);
				b->function = &Interface::e_enviarNuevoPersonaje;
			Boton *bo = new Boton("Nacer");
				bo->Set(20, 210, 90, 20);
				bo->function = &Interface::e_enviarNuevoPersonaje;
			Boton *ca = new Boton("Cancelar");
				ca->Set(120, 210, 90, 20);
				ca->function = &Interface::e_pantallaNuevoPersonaje;
			Selector *seled = new Selector(30, 110);
				seled->addOption("1"); seled->addOption("2"); seled->addOption("3");
				seled->addOption("4"); seled->addOption("5"); seled->addOption("6");
				seled->addOption("7"); seled->addOption("8"); seled->addOption("9");
				seled->addOption("10"); seled->addOption("11");	seled->addOption("12");
				seled->addOption("13"); seled->addOption("14");	seled->addOption("15");
				seled->addOption("16");
			Imagen *n = new Imagen();
				n->Set(230, 50);
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

void Interface::e_enviarNuevoPersonaje(int param)
{
	std::string bff = "1_2_" + this->getContainer(3)->getIB(0)->texto + "_" + this->getContainer(3)->getSelector(0)->getSelected();
    net->sendPacket(bff);

    Label *l = new Label("CREANDO PERSONAJE...", 8);
     	l->Set(50, 120);

	this->getContainer(3)->Add(l);
}

void Interface::e_conectarConPersonaje(int param)
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

void Interface::e_enviarMensaje(int param)
{
	std::string bff = "3_0_" + this->getContainer(0)->getIB(0)->texto;
	net->sendPacket(bff);

    this->closeContainer(0);
    this->writing = false;
    SDL_EnableKeyRepeat(100, 30);
}
