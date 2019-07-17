#include <iostream>
#include <stdexcept>

#include "exceptions.hpp"
#include "scanner.hpp"
#include "tokenizer.hpp"

std::string filename = "example.txt"; // TEMPORARY VALUE
std::string lineFromScanner;

int main(int argc, char* argv[])
{
	Scanner scanner(filename);	
	Tokenizer tokenizer;

	while (true)
	{
		try { lineFromScanner = scanner.scan(); }
		catch (std::exception& e) { std::cout << e.what() << std::endl; break; }

		if (lineFromScanner.empty())
		{
			std::cout << "Scanner returned an empty line" << std::endl;
			break;
		}
		std::cout << lineFromScanner << std::endl;
		tokenizer.tokenize(lineFromScanner);
	}

	system("pause");
	return 0;
}  