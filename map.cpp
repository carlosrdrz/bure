#include "error.h"
#include "map.h"
#include "game.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <libxml/parser.h>

extern game *gameInstance;
extern const std::string path;

map::map(std::string archivo) {
	mapa = NULL;
	elementos = NULL;
	pisable = NULL;

	// Cargamos el mapa y lo iniciamos
	xmlDocPtr doc;
	xmlNodePtr node;
	xmlChar *temp;

	// Cargamos el mapa y lo iniciamos
	doc = xmlParseFile((path+"maps/"+archivo).c_str());
	if (doc  == NULL) {
		Error::Log("** ERROR ** No se pudo cargar uno de los mapas", 2);
	}

	node = xmlDocGetRootElement(doc);
	while(xmlNodeIsText(node)) node = node->next;

	width = atoi((char*)xmlGetProp(node, (xmlChar*)"width"));
	height = atoi((char*)xmlGetProp(node, (xmlChar*)"height"));

	mapa = new int[height*width];
	elementos = new int[height*width];
	pisable = new bool[height*width];

	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;

	int leido, num = 0;

	// Leer primera capa
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;

	while(node != NULL) {
		if(xmlNodeIsText(node)) {
				node = node->next;
				continue;
		}

		temp = xmlGetProp(node, (xmlChar*)"gid");
		mapa[num] = atoi((char*)temp);
		free(temp);
		num++;

		node = node->next;
	}

	node = xmlDocGetRootElement(doc);
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;

	// Leer segunda capa
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;

	leido = 0;
	num = 0;

	while(node != NULL) {
		if(xmlNodeIsText(node)) {
				node = node->next;
				continue;
		}

		temp = xmlGetProp(node, (xmlChar*)"gid");
		elementos[num] = atoi((char*)temp)-98;
		free(temp);
		num++;

		node = node->next;
	}

	node = xmlDocGetRootElement(doc);
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;

	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;

	// Leer tercera capa
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->next;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) node = node->next;

	leido = 0;
	num = 0;

	while(node != NULL) {
		if(xmlNodeIsText(node)) {
				node = node->next;
				continue;
		}

		temp = xmlGetProp(node, (xmlChar*)"gid");
		leido = atoi((char*)temp);
		free(temp);
		if(!leido) pisable[num] = true;
		else pisable[num] = false;
		num++;

		node = node->next;
	}

	xmlFreeNode(node);
	xmlFreeDoc(doc);
}

map::~map() {
	delete[] mapa;
	delete[] elementos;
	delete[] pisable;
}

bool map::comprobarTilePisable(int x, int y)
{
	bool pisabler = pisable[x+(y*width)];

	for(int t = 0; t < gameInstance->jugadores(); t++) {
		player *jd = gameInstance->getPlayerByIndex(t);
		if(jd->x == x && jd->y == y) {
			pisabler = false;
		}
	}

	return pisabler;
}
