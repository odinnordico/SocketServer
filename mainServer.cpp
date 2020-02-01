#include <iostream>

#include "model/Actor.h"
#include "translator/BufferToMessageTranslator.h"
#include "exception/Exception.h"
#include "core/Server.h"
#include "constants/constants.h"

int main() {
	try {
		int port = 3301;
		std::cout << "Used port: " << port << std::endl;
		std::cout << "Broadcast destination " << DESTINATION_BROADCAST
				<< std::endl;
		Actor actor("", port);
		Server server(actor, 1024);
		server.start();
	} catch (Exception &e) {
		std::cout << e.toString() << std::endl;
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

