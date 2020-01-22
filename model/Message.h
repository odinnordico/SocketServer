#ifndef MODEL_MESSAGE_H_
#define MODEL_MESSAGE_H_

#include <string>

#include "Actor.h"

class Message {
public:
	Message();
	virtual ~Message();
	std::string getMessage();
	void setMessage(std::string);
	Actor getSource();
	void setSource(Actor);
	Actor getDestination();
	void setDestination(Actor);

private:
	std::string message;
	Actor source;
	Actor destination;
};

#endif /* MODEL_MESSAGE_H_ */
