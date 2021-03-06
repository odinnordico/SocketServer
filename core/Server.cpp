#include "Server.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <array>

#include "../exception/Exception.h"
#include "../translator/BufferToMessageTranslator.h"
#include "Client.h"

Server::Server() {
	this->socketOptions = 1;
	this->maxQueueBacklogToListen = 10;
	this->bufferSize = 64;
	this->socketAddressLen = 0;
	for (int i = 0; i < MAX_CLIENTS_ALLOWED; i++) {
		this->clients[i] = 0;
	}

}

Server::Server(Actor actor, int bufferSize) {
	this->actor = actor;
	this->socketOptions = 1;
	this->maxQueueBacklogToListen = 10;
	this->bufferSize = bufferSize;
	this->socketAddressLen = 0;

	FD_ZERO(&this->readFds);
	for (int i = 0; i < MAX_CLIENTS_ALLOWED; i++) {
		this->clients[i] = 0;
	}

}

Server::~Server() {
}

void Server::start() {
	this->createSocket();
	this->setSocketOptions();
	this->setSocketAddress();
	this->bindAddress();
	this->listenOnSocket();

	std::cout << "Started Server" << std::endl;
	while (1) {
		this->selectActivity();
		this->socketAddressLen = sizeof(this->socketAddress);
		if (FD_ISSET(this->actor.getSocket(), &this->readFds)) {
			try {
				this->serveNewClient();
			} catch (Exception &e) {
				std::perror(e.toString());
			}
		}
	}
}

void Server::createSocket() {
	int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) {
		throw new Exception("Socket", "Unable to establish socket");
	}
	this->actor.setSocket(socketDescriptor);
	FD_SET(socketDescriptor, &this->readFds);
}

void Server::setSocketOptions() {
	if (setsockopt(this->actor.getSocket(), SOL_SOCKET, SO_REUSEADDR,
			(char*) &this->socketOptions, sizeof(this->socketOptions)) < 0) {
		throw new Exception("Socket Options", "Can not set options on sockets");
	}
}

void Server::setSocketAddress() {
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_addr.s_addr = INADDR_ANY;
	this->socketAddress.sin_port = htons(this->actor.getPort());
}

void Server::bindAddress() {
	if (bind(this->actor.getSocket(), (struct sockaddr*) &this->socketAddress,
			sizeof(this->socketAddress)) < 0) {
		throw new Exception("Bind",
				"Unable to bind this socket to given address");
	}

}

void Server::listenOnSocket() {
	if (listen(this->actor.getSocket(), this->maxQueueBacklogToListen) < 0) {
		throw new Exception("Listen",
				"Unable to set the socket to passive mode to accept connection request");
	}
}

void Server::selectActivity() {
	if (select(this->actor.getSocket() + 1, &this->readFds, NULL,
	NULL, NULL) < 0 && errno != EINTR) {
		std::cout << "ERROR: Error on select to wait for activity" << std::endl;
	}
}

int Server::readBuffer(char buffer[]) {
	return read(this->actor.getSocket(), buffer, this->bufferSize);
}

void Server::serveNewClient() {
	std::thread clientThread(
			[](Server *server, int serverSocket, int sockAddresLen,
					sockaddr_in sockAddress, int bffSize) {
				try {
					int clientSocket;
					if ((clientSocket = accept(serverSocket,
							(struct sockaddr*) &sockAddress,
							(socklen_t*) &sockAddresLen)) < 0) {
						throw new Exception("Accept",
								"Can not accept connection on given socket");
					}
					Server s = *server;
					s.addClientSocket(clientSocket);
					Actor clientActor(inet_ntoa(sockAddress.sin_addr),
							ntohs(sockAddress.sin_port));
					Client client;
					clientActor.setSocket(clientSocket);
					client.init(clientActor, bffSize);
					client.start(s);
				} catch (Exception &e) {
					std::cout << e.toString() << std::endl;
				}
			}, this, this->actor.getSocket(), this->socketAddressLen,
			this->socketAddress, this->bufferSize);
	//this->clients.push_back(std::move(clientThread));
	clientThread.detach();

}

void Server::write(Message message) {
	std::string messageToSend = BufferToMessageTranslator::translateMessage(
			message);
	int messageToSentLen = messageToSend.length();
	int socket = message.getDestination().getSocket();  //. = from
	if (socket == DESTINATION_BROADCAST) {
		std::cout << "Broadcasting message: " << std::endl;
		for (int i = 0; i < MAX_CLIENTS_ALLOWED; i++) {
			std::cout << "ESCANDALAA -  " << i << std::endl;
			if (this->clients[i] > 0) {
				send(this->clients[i], messageToSend.c_str(), messageToSentLen,
						0);
			}
		}
	} else {
		send(socket, messageToSend.c_str(), messageToSentLen, 0);
	}
}

void Server::stopClient(Actor actor) {
	this->removeClientSocket(actor.getSocket());
	close(actor.getSocket());
}

int Server::getMaxQueueBacklogToListen() {
	return this->maxQueueBacklogToListen;
}
void Server::setMaxQueueBacklogToListen(int maxQueueBacklogToListen) {
	this->maxQueueBacklogToListen = maxQueueBacklogToListen;
}

Actor Server::getActor() {
	return this->actor;
}

void Server::stop() {
	close(this->actor.getSocket());
}

void Server::addClientSocket(int socket) {
	for (int i = 0; i < MAX_CLIENTS_ALLOWED; i++) {
		if (this->clients[i] == 0) {
			this->clients[i] = socket;
			break;
		}
	}
}
void Server::removeClientSocket(int socket) {
	for (int i = 0; i < MAX_CLIENTS_ALLOWED; i++) {
		if (this->clients[i] == socket) {
			this->clients[i] = 0;
			break;
		}
	}
}

