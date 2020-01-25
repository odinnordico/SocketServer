#ifndef CORE_SOCKETCLIENT_H_
#define CORE_SOCKETCLIENT_H_

#include <string>

#include "SocketServer.h"
#include "../model/Actor.h"
#include "../model/Message.h"

class SocketClient {
private:
	SocketServer server;
	Actor actor;
	fd_set readFds;
	int bufferSize;

	int readBuffer(char[]);
	void handleDisconnection();
	void selectActivity();

public:
	SocketClient();
	SocketClient(SocketServer&, Actor&, int&);
	virtual ~SocketClient();

	void start();
	void handleNewIncome(char*, int);

};

#endif /* CORE_SOCKETCLIENT_H_ */
