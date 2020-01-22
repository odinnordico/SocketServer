#ifndef TRANSLATOR_BUFFERTOMESSAGETRANSLATOR_H_
#define TRANSLATOR_BUFFERTOMESSAGETRANSLATOR_H_

#include <string>

#include "../model/Message.h"
#include "../json/json.hpp"

using json = nlohmann::json;

class BufferToMessageTranslator {
public:
	BufferToMessageTranslator();
	virtual ~BufferToMessageTranslator();
	static Message translateBuffer(char*);
	static std::string translateMessage(Message);
private:
	static Actor getActorFromJson(const char*, json);
};

#endif /* TRANSLATOR_BUFFERTOMESSAGETRANSLATOR_H_ */
