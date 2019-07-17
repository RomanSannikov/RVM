#include "tokenizer.hpp"

void Tokenizer::tokenize(std::string& lineFromScanner)
{
	auto it_lineFromScannerBegin = std::begin(lineFromScanner);
	auto it_lineFromScannerTempEnd = std::end(lineFromScanner);
	const auto c_it_lineFromScannerEnd = std::end(lineFromScanner);
	
	std::string::iterator it_result;

	bool new_token_was_created = true;

	while (new_token_was_created)
	{
		
	}
}

Token& Tokenizer::recognize(std::string str)
{
	Token newToken;
	return newToken;
}