#ifndef CORE_CLIENT_H_
#define CORE_CLIENT_H_

#include <string>

#include "../model/Actor.h"
#include "../model/Message.h"
#include "Server.h"

class Client {
private:
	Actor actor;
	fd_set readFds;
	int bufferSize;

	int readBuffer(char[]);
	void handleDisconnection(Server&);
	void selectActivity();
	void welcomeClient(Server&);

public:
	Client();
	Client(Actor&, int&);
	void init(Actor&, int&);
	virtual ~Client();

	void start(Server&);
	void handleNewIncome(Server&, char*, int);
	Actor getActor();

};

#endif /* CORE_CLIENT_H_ */
