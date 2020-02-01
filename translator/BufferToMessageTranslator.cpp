#include <iostream>

#include "BufferToMessageTranslator.h"
#include "../constants/constants.h"
#include "../model/Actor.h"

BufferToMessageTranslator::BufferToMessageTranslator() {

}

BufferToMessageTranslator::~BufferToMessageTranslator() {
}

Message BufferToMessageTranslator::translateBuffer(char *buffer) {
	Message message;
	try {
		json jsonMessage = json::parse(buffer);
		message.setAction(jsonMessage[LABEL_ACTION].dump(JSON_IDENTATION));
		message.setMessage(jsonMessage[LABEL_MSG].dump(JSON_IDENTATION));
		Actor source = BufferToMessageTranslator::getActorFromJson(LABEL_SOURCE,
				jsonMessage);
		message.setSource(source);
		Actor destination = BufferToMessageTranslator::getActorFromJson(LABEL_DESTINATION,
				jsonMessage);
		message.setDestination(destination);
	} catch (std::exception &e) {
		std::cout << "ERROR: Unable to parse: " << buffer << std::endl;
	}
	return message;
}
std::string BufferToMessageTranslator::translateMessage(Message message) {
	json jsonMessage;
	jsonMessage[LABEL_ACTION] = message.getAction();
	jsonMessage[LABEL_MSG] = message.getMessage();

	jsonMessage[LABEL_SOURCE][LABEL_SOCKET] = message.getSource().getSocket();
	jsonMessage[LABEL_SOURCE][LABEL_IP] = message.getSource().getIpAddress();
	jsonMessage[LABEL_SOURCE][LABEL_PORT] = message.getSource().getPort();

	jsonMessage[LABEL_DESTINATION][LABEL_SOCKET] =
			message.getDestination().getSocket();
	jsonMessage[LABEL_DESTINATION][LABEL_IP] =
			message.getDestination().getIpAddress();
	jsonMessage[LABEL_DESTINATION][LABEL_PORT] =
			message.getDestination().getPort();

	std::string dumpedJsonMessage = jsonMessage.dump(JSON_IDENTATION);
	return dumpedJsonMessage;

}

Actor BufferToMessageTranslator::getActorFromJson(const char *origination,
		json jsonMessage) {
	std::string ipAddress = jsonMessage[origination][LABEL_IP].dump(
			JSON_IDENTATION);
	int port = std::atoi(
			jsonMessage[origination][LABEL_PORT].dump(JSON_IDENTATION).c_str());
	int socket =
			std::atoi(
					jsonMessage[origination][LABEL_SOCKET].dump(JSON_IDENTATION).c_str());
	Actor actor(ipAddress, port);
	actor.setSocket(socket);
	return actor;
}

