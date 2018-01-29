#include "Config.h"
#include "Error.h"

#include "libxml/parser.h"
#include "libxml/tree.h"
#include <string>

extern std::string path;

Config::Config() {
	if ((doc = xmlParseFile((path+"data/config.xml").c_str())) == NULL) {
		Error::Log("No se pudo cargar el archivo config.xml", 3);
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}
}

Config::~Config() {
	xmlFreeNode(node);
	xmlFreeDoc(doc);
}

std::string Config::getValueOf(std::string of) {
	std::string toreturn;
	xmlChar *temp;

	while(node != NULL) {
		if(!of.compare((char*)node->name)) {
			temp = xmlGetProp(node, (xmlChar*)"value");
			toreturn =  (char*)temp;
			free(temp);
			break;
		} else {
			// Avanzar
			node = node->next;
			while(xmlNodeIsText(node)) {
				node = node->next;
			}
		}
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}

	return toreturn;
}

bool Config::getBoolValueOf(std::string of) {
	bool toreturn = true;
	std::string cero = "0";
	xmlChar *temp;

	while(node != NULL) {
		if(!of.compare((char*)node->name)) {
			xmlChar *temp = xmlGetProp(node, (xmlChar*)"value");
			if(!cero.compare((char*)temp)) {
				toreturn = false;
			}
			break;
		} else {
			// Avanzar
			node = node->next;
			while(xmlNodeIsText(node)) {
				node = node->next;
			}
		}
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}

	return toreturn;
}

void Config::setValueOf(std::string of, std::string on) {
	while(node != NULL) {
		if(!of.compare((char*)node->name)) {
			// Cambiar
			xmlSetProp(node, (xmlChar*)"value", (xmlChar*)on.c_str());
			xmlSaveFileEnc((path+"data/config.xml").c_str(), doc, "UTF-8");
			break;
		} else {
			// Avanzar
			node = node->next;
			while(xmlNodeIsText(node)) {
				node = node->next;
			}
		}
	}

	node = xmlDocGetRootElement(doc);
	node = node->xmlChildrenNode;
	while(xmlNodeIsText(node)) {
		node = node->next;
	}
}
