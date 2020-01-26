//busca archivos existentes
#include "Client.h"
//lib c++
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
//busca archivos existentes
#include "../model/Message.h"
#include "../translator/BufferToMessageTranslator.h"
#include "../exception/Exception.h"

Client::Client() {
	this->bufferSize = 64;
}

Client::Client(Server &server, Actor &actor, int &bufferSize) {
	this->actor = actor;
	this->bufferSize = bufferSize;
	this->server = server;
	FD_ZERO(&this->readFds);
}

Client::~Client() {
}

void Client::start() {
	FD_SET(this->actor.getSocket(), &this->readFds);

	char buffer[this->bufferSize];
	while (1) {
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
}

void Client::selectActivity() {
	if (select(this->actor.getSocket() + 1, &this->readFds, NULL,
	NULL, NULL) < 0 && errno != EINTR) {
		throw new Exception("select fail",
				"Error on select to wait for activity");
	}
}

int Client::readBuffer(char buffer[]) {
	return read(this->actor.getSocket(), buffer, this->bufferSize);
}

void Client::handleDisconnection() {
	std::cout << "Host disconnected:" << "\n\tSOCKET: "
			<< this->actor.getSocket() << "\n\tIP: "
			<< this->actor.getIpAddress() << "\n\tPORT: "
			<< this->actor.getPort() << std::endl;
	close(this->actor.getSocket());
	throw new Exception("Client off","Cliente desconectado");
}

void Client::handleNewIncome(char *buffer, int readedValue) {
	buffer[readedValue] = '\0';
	Message message = BufferToMessageTranslator::translateBuffer(buffer);
	server.write(message);
}
