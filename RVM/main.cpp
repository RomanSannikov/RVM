#include <iostream>

#include "scanner.hpp"
#include "tokenizer.hpp"

std::string filename = "example.txt"; // Fix: temporary value
std::string lineFromScanner;


void startVirtualMachine()
{
	Scanner scanner(filename);
	Tokenizer tokenizer;

	while (true)
	{
		lineFromScanner = scanner.getLine();

		if (lineFromScanner.empty())
			continue;

		std::cout << lineFromScanner << std::endl;
		
		tokenizer.tokenize(lineFromScanner);
	}
}


int main(int argc, char* argv[])
{
	startVirtualMachine();

	system("pause");
	return 0;
}  