#include "SocketClient.h"

SocketClient::SocketClient(Actor actor) {
	this->actor = actor;
	this->id = 0;
}

SocketClient::~SocketClient() {
}

int SocketClient::getId() {
	return this->id;
}
void SocketClient::setId(int) {
	this->id = id;
}
void SocketClient::connect(Actor actor) {
}

