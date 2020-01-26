#include <string>

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

class Exception: public std::exception {      //: ES HERENCIA, HEREDE DE LA CLASE publica exception
private:
	std::string summary;
	std::string message;
public:
	Exception(std::string, std::string);
	virtual ~Exception();

	std::string getSummary();
	std::string getMessage();

};

#endif /* EXCEPTION_H_ */
