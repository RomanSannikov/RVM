#include <iostream>
#include <stdexcept>
#include <thread>

#include "exceptions.hpp"
#include "scanner.hpp"
#include "tokenizer.hpp"

std::string filename = "example.txt"; // TEMPORARY VALUE
std::string lineFromScanner;


void startVirtualMachine()
{
	Scanner scanner(filename);
	Tokenizer tokenizer;

	while (true)
	{
		lineFromScanner = scanner.getLine();

		if (lineFromScanner.empty())
		{
			std::cout << "Scanner returned an empty line" << std::endl;
			break;
		}
		std::cout << lineFromScanner << std::endl;
		tokenizer.tokenize(lineFromScanner);
		std::this_thread::yield();
	}
}


int main(int argc, char* argv[])
{
	startVirtualMachine();

	system("pause");
	return 0;
}  