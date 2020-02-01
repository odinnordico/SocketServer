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
#include "../constants/constants.h"

Client::Client() {
	this->bufferSize = 64;
}

Client::Client(Actor &actor, int &bufferSize) {
	this->init(actor, bufferSize);
}

void Client::init(Actor &actor, int &bufferSize) {
	this->actor = actor;
	this->bufferSize = bufferSize;
	FD_ZERO(&this->readFds);
}

Client::~Client() {
}

void Client::start(Server &server) {
	FD_SET(this->actor.getSocket(), &this->readFds);

	char buffer[this->bufferSize];
	this->welcomeClient(server);
	while (1) {
		this->selectActivity();
		if (FD_ISSET(this->actor.getSocket(), &this->readFds)) {
			int readedValue = this->readBuffer(buffer);
			if (readedValue == 0) {
				this->handleDisconnection(server);
				break;
			} else {
				this->handleNewIncome(server, buffer, readedValue);
			}
		}
	}
}

void Client::welcomeClient(Server &server) {
	Message welcomeMessage;
	welcomeMessage.setAction(CHAT_ACTION_START);
	welcomeMessage.setMessage(CHAT_ACTION_START);
	welcomeMessage.setDestination(this->actor);
	welcomeMessage.setSource(server.getActor());
	server.write(welcomeMessage);
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

void Client::handleDisconnection(Server &server) {
	std::cout << "Host disconnected:" << "\n\tSOCKET: "
			<< this->actor.getSocket() << "\n\tIP: "
			<< this->actor.getIpAddress() << "\n\tPORT: "
			<< this->actor.getPort() << std::endl;
	server.stopClient(this->actor);
}

void Client::handleNewIncome(Server &server, char *buffer, int readedValue) {
	if (readedValue > 2) {
		buffer[readedValue] = '\0';
		Message message = BufferToMessageTranslator::translateBuffer(buffer);
		if (message.getDestination().getSocket() > 0) {
			server.write(message);
		} else {
			std::cout << buffer << std::endl;
		}
	}
}

Actor Client::getActor() {
	return this->actor;
}
