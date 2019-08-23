#include <iostream>

#include "scanner.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"


void startVirtualMachine()
{
	const std::string c_filename = "example.txt"; // Fix: temporary value
	
	Scanner scanner(c_filename);
	Tokenizer tokenizer;
	Parser parser;
	
	std::string lineFromScanner;

	while (!scanner.isEOF())
	{
		scanner.getLine(lineFromScanner);

		if (lineFromScanner.empty())
			continue;

		std::cout << lineFromScanner << std::endl;
		
		tokenizer.tokenize(lineFromScanner, scanner.getLineNumber());

		parser.parse(tokenizer.tokens);
	}

	parser.completeParsing();

	parser.printInstructions();
}


int main(int argc, char* argv[])
{
	startVirtualMachine();

	system("pause");
	return 0;
}  