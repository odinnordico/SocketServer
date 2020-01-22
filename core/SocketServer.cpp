#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "SocketServer.h"
#include "../exception/Exception.h"

SocketServer::SocketServer(Actor actor) {
	this->actor = actor;
	this->socketOptions = 1;
}

SocketServer::~SocketServer() {
}

void SocketServer::start() {
	this->createSocket();
	this->setSocketOptions();

}

void SocketServer::createSocket() {
	int socket = socket(AF_INET, SOCK_STREAM, 0);
	if (socket == 0) {
		throw new Exception("Socket", "Unable to establish socket");
	}
	this->actor.setSocket(socket);
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

void SocketServer::stop() {

}

