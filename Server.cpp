#include "model/Actor.h"
#include "translator/BufferToMessageTranslator.h"
#include "exception/Exception.h"
#include "core/Server.h"

int main(){
	Actor actor("",3300);
	Server server(actor,1024);
	server.start();

}


