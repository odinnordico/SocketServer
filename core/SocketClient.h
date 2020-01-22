#ifndef CORE_SOCKETCLIENT_H_
#define CORE_SOCKETCLIENT_H_

#include <string>

#include "../model/Actor.h"

class SocketClient {
private:
	Actor actor;
	int id;
public:
	SocketClient(Actor actor);
	virtual ~SocketClient();
	int getId();
	void setId(int);
	void connect(Actor actor);
};

#endif /* CORE_SOCKETCLIENT_H_ */
