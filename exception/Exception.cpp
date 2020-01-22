#include <string>
#include "Exception.h"

Exception::Exception(std::string summary, std::string message) {
	this->summary = summary;
	this->message = message;
}

Exception::~Exception() {
}

