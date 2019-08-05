#include <iostream>

#include "scanner.hpp"
#include "tokenizer.hpp"


void startVirtualMachine()
{
	const std::string c_filename = "example.txt"; // Fix: temporary value
	
	Scanner scanner(c_filename);
	Tokenizer tokenizer;
	
	std::string lineFromScanner;

	while (true)
	{
		scanner.getLine(lineFromScanner);

		if (scanner.isEOF())
			break;
		else if (lineFromScanner.empty())
			continue;

		std::cout << lineFromScanner << std::endl;
		
		tokenizer.tokenize(lineFromScanner);
	
		tokenizer.testTokens();
	}
}


int main(int argc, char* argv[])
{
	startVirtualMachine();

	system("pause");
	return 0;
}  