#include "tokenizer.hpp"

void Tokenizer::tokenize(std::string& lineFromScanner)
{
	bool keepTokenizing = true;
	unsigned currentPoint = 0;
	std::string stringSlice;
	StringSlice slice;
	Token newToken;

	while (keepTokenizing) // OPTIMIZE: replace with true
	{
		slice = is_(lineFromScanner, currentPoint);

		if (slice.length == 0 || currentPoint >= lineFromScanner.size())
			break; //keepTokenizing = false;

		stringSlice = lineFromScanner.substr(currentPoint, slice.length);
		newToken.stringValue = stringSlice;

		std::cout << stringSlice << std::endl; // temp
		
		if (slice.mode == c_WORD || slice.mode == c_PUNCTUATIONSYMBOL) newToken = recognize(stringSlice);
		else if (slice.mode == c_NUMBER) newToken.tokenState = TokenState::number;

		currentPoint += slice.length;
	}
}


Token& Tokenizer::recognize(const std::string& str)
{
	Token newToken = {TokenState::op_add, str};
	return newToken;
}


Tokenizer::StringSlice& Tokenizer::is_(const std::string& line, const int& startPoint)
{
	StringSlice slice = {0};

	if (isalpha(line[startPoint]))
		slice.mode = c_WORD;
	else if (isdigit(line[startPoint]))
		slice.mode = c_NUMBER;
	else
	{
		for(auto i : c_punctuationSymbols) if (i == line[startPoint]) return (slice = {c_PUNCTUATIONSYMBOL, 1});
		return (slice = {c_PUNCTUATIONSYMBOL, 0});
	}

	++slice.length;

	for (unsigned i = startPoint + 1; i < line.size(); ++i)
	{
		if (slice.mode == c_WORD && isalpha(line[i])) ++slice.length;
		else if (slice.mode == c_NUMBER && isdigit(line[i])) ++slice.length;
		else break;
	}

	return slice;	
}