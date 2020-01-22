#include "Message.h"

Message::Message(){
}

Message::~Message() {
}

std::string Message::getMessage() {
	return this->message;
}
void Message::setMessage(std::string message) {
	this->message = message;
}
Actor Message::getSource() {
	return this->source;
}
void Message::setSource(Actor source) {
	this->source = source;
}
Actor Message::getDestination() {
	return this->destination;
}
void Message::setDestination(Actor destination) {
	this->destination = destination;
}
