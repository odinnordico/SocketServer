#ifndef CORE_SOCKETSERVER_H_
#define CORE_SOCKETSERVER_H_

#include <string>
#include <netinet/in.h>

#include "../model/Actor.h"
#include "SocketClient.h"

class SocketServer {
private:
	Actor actor;
	int socketOptions;
	struct sockaddr_in socketAddress;
	SocketClient clients[];

	void createSocket();
	void setSocketOptions();
	void setSocketAddress();
public:
	SocketServer(Actor actor);
	virtual ~SocketServer();
	void start();
	void stop();
};

#endif /* CORE_SOCKETSERVER_H_ */
