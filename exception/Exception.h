#include <string>

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

class Exception: public std::exception {
public:
	Exception(std::string, std::string);
	virtual ~Exception();

	std::string getSummary();
	std::string getMessage();
private:
	std::string summary;
	std::string message;
};

#endif /* EXCEPTION_H_ */
