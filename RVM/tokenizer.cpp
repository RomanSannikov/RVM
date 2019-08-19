#include "tokenizer.hpp"


void Tokenizer::tokenize(const std::string& c_lineFromScanner)
{
	unsigned currentPoint = 0;
	std::string stringSlice;
	StringSlice slice;
	Token newToken = { TokenState::op_add };

	tokens.clear();

	while (true)
	{
		slice.length = 0;

		slice = getSlice(c_lineFromScanner, currentPoint, slice);

		if (slice.length == 0 || currentPoint >= c_lineFromScanner.size())
			break;

		stringSlice = c_lineFromScanner.substr(currentPoint, slice.length);
		newToken.stringValue = stringSlice;

		recognize(newToken, slice.mode);

		currentPoint += slice.length;
	
		tokens.push_back(newToken);
	}
}


void Tokenizer::recognize(Token& token, const uint8_t& c_mode)
{
	if (c_mode == c_WORD)
	{
		if (token.stringValue[token.stringValue.size() - 1] == ':')
			token.tokenState = TokenState::word;
		else
			recognizeToken(token, c_stringInstructions, TokenState::op_add);
	}
	else if (c_mode == c_PUNCTUATIONSYMBOL)
		recognizeToken(token, c_punctuationSymbols, TokenState::space);
	else if (c_mode == c_NUMBER)
		token.tokenState = TokenState::number;
}


void Tokenizer::recognizeToken(Token& token, const std::vector<std::string>& array, const TokenState tokenState)
{
	for (unsigned i = 0; i < array.size(); ++i)
	{
		if (token.stringValue == array[i])
		{
			token.tokenState = static_cast<TokenState>(static_cast<int>(tokenState) + i);
			return;
		}
	}

	token.tokenState = TokenState::word;
}


Tokenizer::StringSlice& Tokenizer::getSlice(const std::string& c_line, const unsigned& c_startPoint, StringSlice& slice)
{
	if (isalpha(c_line[c_startPoint]))
		slice.mode = c_WORD;
	else if (isdigit(c_line[c_startPoint]))
		slice.mode = c_NUMBER;
	else
	{
		for(auto& i : c_punctuationSymbols) if (i[0] == c_line[c_startPoint]) return (slice = {1, c_PUNCTUATIONSYMBOL });
		return (slice = {0, c_PUNCTUATIONSYMBOL});
	}

	++slice.length;

	for (unsigned i = c_startPoint + 1; i < c_line.size(); ++i)
	{
		if ((slice.mode == c_WORD) && (isalpha(c_line[i]) || isdigit(c_line[i]) || c_line[i] == ':')) ++slice.length;
		else if (slice.mode == c_NUMBER && isdigit(c_line[i])) ++slice.length;
		else break;
	}

	return slice;	
}