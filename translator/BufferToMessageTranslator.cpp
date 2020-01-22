#include "../constants/constants.h"
#include "../model/Actor.h"
#include "BufferToMessageTranslator.h"

BufferToMessageTranslator::BufferToMessageTranslator() {
	// TODO Auto-generated constructor stub

}

BufferToMessageTranslator::~BufferToMessageTranslator() {
	// TODO Auto-generated destructor stub
}

Message BufferToMessageTranslator::translateBuffer(char *buffer) {
	json jsonMessage = json::parse(buffer);
	Message message;
	message.setAction(jsonMessage[LABEL_ACTION].dump(JSON_IDENTATION));
	message.setMessage(jsonMessage[LABEL_CLIENT].dump(JSON_IDENTATION));
	message.setSource(
			BufferToMessageTranslator::getActorFromJson(LABEL_SOURCE,
					jsonMessage));
	message.setSource(
			BufferToMessageTranslator::getActorFromJson(LABEL_DESTINATION,
					jsonMessage));

	return message;

}
std::string BufferToMessageTranslator::translateMessage(Message message) {
	json jsonMessage;
	jsonMessage[LABEL_ACTION] = message.getAction();
	jsonMessage[LABEL_MSG] = message.getMessage();

	jsonMessage[LABEL_SOURCE][LABEL_SOCKET] = message.getSource().getSocket();
	jsonMessage[LABEL_SOURCE][LABEL_IP] = message.getSource().getIpAddress();
	jsonMessage[LABEL_SOURCE][LABEL_PORT] = message.getSource().getPort();

	jsonMessage[LABEL_DESTINATION][LABEL_SOCKET] = message.getDestination().getSocket();
	jsonMessage[LABEL_DESTINATION][LABEL_IP] = message.getDestination().getIpAddress();
	jsonMessage[LABEL_DESTINATION][LABEL_PORT] = message.getDestination().getPort();

	std::string dumpedJsonMessage = jsonMessage.dump(JSON_IDENTATION);
	return dumpedJsonMessage;

}

Actor BufferToMessageTranslator::getActorFromJson(const char *origination,
		json jsonMessage) {
	std::string ipAddress = jsonMessage[origination][LABEL_IP].dump(
			JSON_IDENTATION);
	int port = std::atoi(
			jsonMessage[origination][LABEL_PORT].dump(JSON_IDENTATION).c_str());
	int socket = std::atoi(
			jsonMessage[origination][LABEL_PORT].dump(JSON_IDENTATION).c_str());

	Actor actor(ipAddress, port);
	actor.setSocket(socket);
	return actor;
}

