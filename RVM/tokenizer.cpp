#include "tokenizer.hpp"

void Tokenizer::tokenize(const std::string& lineFromScanner)
{
	bool keepTokenizing = true; // FIX: get rid of this
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
		
		recognize(newToken, slice.mode);

		if (newToken.stringValue.empty())
		{
			std::cout << "not recognized" << std::endl;
			break;
		}

		currentPoint += slice.length;
	}
}


void Tokenizer::recognize(Token& token, const uint8_t& c_mode)
{
	if (c_mode == c_WORD)
	{
		for (int i = 0; i < c_stringTokenState.size(); ++i)
		{
			if (token.stringValue == c_stringTokenState[i])
			{
				token.tokenState = static_cast<TokenState>(static_cast<int>(TokenState::op_add) + i);
				break;
			}
		}

		if (token.stringValue[token.stringValue.size() - 1] == ':')
			token.tokenState = TokenState::label;
	}
	else if (c_mode == c_PUNCTUATIONSYMBOL)
	{
		for (int i = 0; i < c_punctuationSymbols.size(); ++i)
			if (token.stringValue == c_stringTokenState[i])
				token.tokenState = static_cast<TokenState>(static_cast<int>(TokenState::space) + i);
	}
	else if (c_mode == c_NUMBER)
		token.tokenState = TokenState::number;
	else
		token.stringValue.clear();
}


Tokenizer::StringSlice& Tokenizer::is_(const std::string& c_line, const int& c_startPoint)
{
	StringSlice slice = {0, 0};

	if (isalpha(c_line[c_startPoint]))
		slice.mode = c_WORD;
	else if (isdigit(c_line[c_startPoint]))
		slice.mode = c_NUMBER;
	else
	{
		for(auto i : c_punctuationSymbols) if (i == c_line[c_startPoint]) return (slice = {1, c_PUNCTUATIONSYMBOL });
		return (slice = {0, c_PUNCTUATIONSYMBOL});
	}

	++slice.length;

	for (unsigned i = c_startPoint + 1; i < c_line.size(); ++i)
	{
		if (slice.mode == c_WORD && isalpha(c_line[i])) ++slice.length;
		else if (slice.mode == c_NUMBER && isdigit(c_line[i])) ++slice.length;
		else break;
	}

	return slice;	
}