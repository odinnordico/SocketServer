#ifndef CORE_SOCKETSERVER_H_
#define CORE_SOCKETSERVER_H_

#include <string>
#include <netinet/in.h>

#include "../model/Actor.h"
#include "../model/Message.h"

class SocketServer {
private:
	Actor actor;
	int socketOptions;
	struct sockaddr_in socketAddress;
	int maxQueueBacklogToListen;
	fd_set readFds;
	int bufferSize;
	int socketAddressLen;

	void createSocket();
	void setSocketOptions();
	void setSocketAddress();
	void bindAddress();
	void listenOnSocket();
	void selectActivity();
	void serveNewClient();
	int readBuffer(char[]);

public:
	SocketServer();
	SocketServer(Actor, int);
	virtual ~SocketServer();
	void start();
	void stop();
	void write(Message);

	int getMaxQueueBacklogToListen();
	void setMaxQueueBacklogToListen(int);
};

#endif /* CORE_SOCKETSERVER_H_ */
