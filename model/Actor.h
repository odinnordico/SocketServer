#ifndef CORE_ACTOR_H_
#define CORE_ACTOR_H_

#include <string>

class Actor {
private:
	int socket;
	std::string ipAddress;
	int port;
public:
	Actor(std::string, int);
	virtual ~Actor();
	int getSocket();
	void setSocket(int);
	std::string getIpAddress();
	void setIpAddress(std::string);
	int getPort();
	void setPort(int);

	void write(std::string, int, std::string);
	std::string read(std::string, int);
};

#endif /* CORE_ACTOR_H_ */
