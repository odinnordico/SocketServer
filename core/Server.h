#ifndef CORE_SERVER_H_
#define CORE_SERVER_H_

#include <string>
#include <vector>
#include <netinet/in.h>

#include "../model/Actor.h"
#include "../model/Message.h"

class Server {
private:
	Actor actor;
	int socketOptions;
	struct sockaddr_in socketAddress;
	int maxQueueBacklogToListen;
	fd_set readFds;    //bag empall
	int bufferSize;
	int socketAddressLen;
	std::vector<Actor> clients;

	void createSocket();
	void setSocketOptions();
	void setSocketAddress();
	void bindAddress();
	void listenOnSocket();
	void selectActivity();
	void serveNewClient();
	int readBuffer(char[]);

public:
	Server();
	Server(Actor, int);
	virtual ~Server();
	void start();
	void stop();
	void write(Message);
	void stopClient(Actor);


	int getMaxQueueBacklogToListen();
	void setMaxQueueBacklogToListen(int);
	Actor getActor();
};

#endif /* CORE_SERVER_H_ */
