#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <SDL_net.h>
#include <string>
#include <list>

class Network {
public:
	int status;
	int intentoConexion;

	Network(void);
	~Network(void);

	int connectToTheServer();
	void sendPacket(std::string pa);

	// Bucles de envio y recibo
	void sendLoop();
	void recieveLoop();

	// Traducir paquetes
	void traducirPaquete(std::string data);
	void t_personajeSeFue(std::string data);
	void t_respuestaLogin(std::string data);
	void t_obtenerlistaPersonajes(std::string data);
	void t_respuestaCrearPersonaje(std::string data);
	void t_respuestaIdentificado(std::string data);
	void t_nuevoJugadorConectado(std::string data);
	void t_jugadorSeMovio(std::string data);
	void t_mensajeEnviado(std::string data);
	void t_expulsadoDelServer(std::string data);
	void t_cambiarMapa(std::string data);

private:
	UDPsocket sock;
	UDPpacket *packenv, *packrec;
	IPaddress srv;

	std::list<std::string> paraEnviar;
};

#endif
