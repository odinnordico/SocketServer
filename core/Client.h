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
	void welcomeClient();

public:
	Client();
	Client(Server&, Actor&, int&);
	void init(Server&, Actor&, int&);
	virtual ~Client();

	void start();
	void handleNewIncome(char*, int);
	Actor getActor();

};

#endif /* CORE_CLIENT_H_ */
