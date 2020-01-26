#ifndef MODEL_MESSAGE_H_
#define MODEL_MESSAGE_H_

#include <string>

#include "Actor.h"

class Message {
private:
	std::string action;
	std::string message;
	Actor source;
	Actor destination;
public:
	Message();
	virtual ~Message();
	std::string getAction();
	void setAction(std::string);
	std::string getMessage();
	void setMessage(std::string);
	Actor getSource();
	void setSource(Actor);
	Actor getDestination();
	void setDestination(Actor);


};

#endif /* MODEL_MESSAGE_H_ */
