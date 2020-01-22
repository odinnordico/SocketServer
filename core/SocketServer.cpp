#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#include "SocketServer.h"
#include "SocketClient.h"
#include "../exception/Exception.h"
#include "../translator/BufferToMessageTranslator.h"

SocketServer::SocketServer() {
	this->socketOptions = 1;
	this->maxQueueBacklogToListen = 10;
	this->bufferSize = 64;
	this->socketAddressLen = 0;
}

SocketServer::SocketServer(Actor actor, int bufferSize) {
	this->actor = actor;
	this->socketOptions = 1;
	this->maxQueueBacklogToListen = 10;
	this->bufferSize = bufferSize;
	this->socketAddressLen = 0;

	FD_ZERO(&this->readFds);

}

SocketServer::~SocketServer() {
}

void SocketServer::start() {
	this->createSocket();
	this->setSocketOptions();
	this->setSocketAddress();
	this->bindAddress();
	this->listenOnSocket();
	while (1) {
		this->selectActivity();
		this->socketAddressLen = sizeof(this->socketAddress);
		if (FD_ISSET(this->actor.getSocket(), &this->readFds)) {
			std::thread thread(this->serveNewClient);
		}
	}

}

void SocketServer::createSocket() {
	int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor < 0) {
		throw new Exception("Socket", "Unable to establish socket");
	}
	this->actor.setSocket(socketDescriptor);
	FD_SET(socketDescriptor, &this->readFds);
}

void SocketServer::setSocketOptions() {
	if (setsockopt(this->actor.getSocket(), SOL_SOCKET, SO_REUSEADDR,
			(char*) &this->socketOptions, sizeof(this->socketOptions)) < 0) {
		throw new Exception("Socket Options", "Can not set options on sockets");
	}
}

void SocketServer::setSocketAddress() {
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_addr.s_addr = INADDR_ANY;
	this->socketAddress.sin_port = htons(this->actor.getPort());
}

void SocketServer::bindAddress() {
	if (bind(this->actor.getSocket(), (struct sockaddr*) &this->socketAddress,
			sizeof(this->socketAddress)) < 0) {
		throw new Exception("Bind",
				"Unable to bind this socket to given address");
	}
}

void SocketServer::listenOnSocket() {
	if (listen(this->actor.getSocket(), this->maxQueueBacklogToListen) < 0) {
		throw new Exception("Listen",
				"Unable to set the socket to passive mode to accept connection request");
	}
}

void SocketServer::selectActivity() {
	if (select(this->actor.getSocket() + 1, &this->readFds, NULL,
	NULL, NULL) < 0 && errno != EINTR) {
		perror("Error on select to wait for activity");
	}
}

int SocketServer::readBuffer(char buffer[]) {
	return read(this->actor.getSocket(), buffer, this->bufferSize);
}

void SocketServer::serveNewClient() {
	int clientSocket;
	if ((clientSocket = accept(this->actor.getSocket(),
			(struct sockaddr*) &this->socketAddress,
			(socklen_t*) &this->socketAddressLen)) < 0) {
		throw new Exception("Accept",
				"Can not accept connection on given socket");
	}
	Actor clientActor(inet_ntoa(this->socketAddress.sin_addr),
			ntohs(this->socketAddress.sin_port));
	clientActor.setSocket(clientSocket);
	SocketClient client(*this, clientActor, this->bufferSize);
	client.start();
}

void SocketServer::write(Message message) {
	const char *messageToSend = BufferToMessageTranslator::translateMessage(
			message).c_str();
	int messageToSentLen = strlen(messageToSend);
	int socket = message.getDestination().getSocket();
	send(socket, messageToSend, messageToSentLen, 0);
}

int SocketServer::getMaxQueueBacklogToListen() {
	return this->maxQueueBacklogToListen;
}
void SocketServer::setMaxQueueBacklogToListen(int maxQueueBacklogToListen) {
	this->maxQueueBacklogToListen = maxQueueBacklogToListen;
}

void SocketServer::stop() {
	close(this->actor.getSocket());
}

