#ifndef CORE_CLIENT_H_
#define CORE_CLIENT_H_

#include <string>

#include "../model/Actor.h"
#include "../model/Message.h"
#include "Server.h"

class Client {
private:
	Server server;
	Actor actor;
	fd_set readFds;
	int bufferSize;

	int readBuffer(char[]);
	void handleDisconnection();
	void selectActivity();

public:
	Client();
	Client(Server&, Actor&, int&);
	virtual ~Client();

	void start();
	void handleNewIncome(char*, int);

};

#endif /* CORE_CLIENT_H_ */
