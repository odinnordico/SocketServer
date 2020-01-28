#include <string>
#include "Exception.h"

Exception::Exception(std::string summary, std::string message) {
	this->summary = summary;
	this->message = message;
}

Exception::~Exception() {
}

std::string Exception::getSummary() {
	return this->summary;
}

std::string Exception::getMessage() {
	return this->message;
}

const char* Exception::toString() {
	std::string errorMessage("ERROR:");
	errorMessage.append("\n\tSummary: ");
	errorMessage.append(this->summary);
	errorMessage.append("\n\tMessage:\n");
	errorMessage.append(this->message);
	errorMessage.append("\n");
	return errorMessage.c_str();
}
