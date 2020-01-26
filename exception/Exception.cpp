#include <string>
#include "Exception.h"

Exception::Exception(std::string summary, std::string message) {
	this->summary = summary;
	this->message = message;
}

Exception::~Exception() {
}

std::string Exception::getSummary(){
	return this->summary;
}

std::string Exception::getMessage(){
	return this->message;
}
