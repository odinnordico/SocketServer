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
	this->welcomeClient();
	while (1) {
		this->selectActivity();
		if (FD_ISSET(this->actor.getSocket(), &this->readFds)) {
			int readedValue = this->readBuffer(buffer);
			if (readedValue == 0) {
				this->handleDisconnection();
				break;
			} else {
				this->handleNewIncome(buffer, readedValue);
			}
		}
	}
}

void Client::welcomeClient(){
	Message welcomeMessage;
	welcomeMessage.setAction(CHAT_ACTION_START);
	welcomeMessage.setMessage(CHAT_ACTION_START);
	welcomeMessage.setDestination(this->actor);
	welcomeMessage.setSource(this->server.getActor());
	this->server.write(welcomeMessage);
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
}

void Client::handleNewIncome(char *buffer, int readedValue) {
	buffer[readedValue] = '\0';
	Message message = BufferToMessageTranslator::translateBuffer(buffer);
	if(message.getDestination().getSocket() > 0){
		this->server.write(message);
	} else {
		std::cout << buffer << std::endl;
	}
}
