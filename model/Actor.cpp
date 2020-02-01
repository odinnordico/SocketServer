#include <string>
#include "Actor.h"

Actor::Actor() {
	this->port = 0;
	this->socket = 0;
}

Actor::Actor(std::string ipAddress, int port) {
	this->ipAddress = ipAddress;
	this->port = port;
	this->socket = 0;
}

Actor::~Actor() {
}

int Actor::getSocket() {
	return this->socket;
}
void Actor::setSocket(int socket) {
	this->socket = socket;
}
std::string Actor::getIpAddress() {
	return this->ipAddress;
}
void Actor::setIpAddress(std::string ipAddress) {
	this->ipAddress = ipAddress;
}
int Actor::getPort() {
	return this->port;
}
void Actor::setPort(int port) {
	this->port = port;
}

