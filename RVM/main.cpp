#include <iostream>
#include <stdexcept>

#include "exceptions.hpp"
#include "scanner.hpp"
#include "main.h"

std::string filename = "example.txt"; // TEMPORARY VALUE
std::string lineFromScanner;

int main(int argc, char* argv[])
{
	Scanner scanner(filename);	
	Tokenizer tokenizer();

	try
	{
		while (true)
		{
			lineFromScanner = scanner.scan();
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	system("pause");
	return 0;
}  