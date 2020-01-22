#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "SocketClient.h"
#include "../model/Message.h"
#include "../translator/BufferToMessageTranslator.h"

SocketClient::SocketClient() {
	this->bufferSize = 64;
}

SocketClient::SocketClient(SocketServer &server, Actor actor, const int bufferSize) {
	this->actor = actor;
	this->bufferSize = bufferSize;
	this->server = server;
	FD_ZERO(&this->readFds);
}

SocketClient::~SocketClient() {
}

void SocketClient::start() {
	FD_SET(this->actor.getSocket(), &this->readFds);

	char buffer[this->bufferSize];

	this->selectActivity();
	if (FD_ISSET(this->actor.getSocket(), &this->readFds)) {
		int readedValue = this->readBuffer(buffer);
		if (readedValue == 0) {
			this->handleDisconnection();
		} else {
			this->handleNewIncome(buffer, readedValue);
		}
	}
}

void SocketClient::selectActivity() {
	if (select(this->actor.getSocket() + 1, &this->readFds, NULL,
	NULL, NULL) < 0 && errno != EINTR) {
		perror("Error on select to wait for activity");
	}
}

int SocketClient::readBuffer(char buffer[]) {
	return read(this->actor.getSocket(), buffer, this->bufferSize);
}

void SocketClient::handleDisconnection() {
	std::cout << "Host disconnected:" << "\n\tSOCKET: "
			<< this->actor.getSocket() << "\n\tIP: "
			<< this->actor.getIpAddress() << "\n\tPORT: "
			<< this->actor.getPort() << std::endl;
	close(this->actor.getSocket());
}

void SocketClient::handleNewIncome(char *buffer, int readedValue) {
	buffer[readedValue] = '\0';
	Message message = BufferToMessageTranslator::translateBuffer(buffer);
	server.write(message);
}
