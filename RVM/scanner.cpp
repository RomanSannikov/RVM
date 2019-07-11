#include "scanner.hpp"


inline void Scanner::open(const std::string& filename)
{
	file.open(filename);
}


inline void Scanner::open(const char* const filename)
{
	file.open(filename);
}


inline bool Scanner::isOpen()
{
	return file.is_open();
}


std::string Scanner::scan()
{
	if (!file.is_open)
		throw std::exception();

	char* scannedLine = (char*)malloc(c_lineSize);
	file.getline(scannedLine, c_lineSize);

	return scannedLine;
}