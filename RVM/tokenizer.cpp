#include "tokenizer.hpp"


void Tokenizer::tokenize(const std::string& c_lineFromScanner, const unsigned& c_lineNumber)
{
	unsigned currentPoint = 0;
	StringSlice slice;
	Token newToken = { TokenState::op_add };

	tokens.clear();

	while (true)
	{
		slice.length = 0;

		slice = getSlice(c_lineFromScanner, currentPoint, slice);

		if (slice.length == 0 || currentPoint >= c_lineFromScanner.size())
			break;
		else if (slice.mode == c_PUNCTUATIONSYMBOL)
		{
			currentPoint += slice.length;
			continue;
		}

		newToken.stringValue = c_lineFromScanner.substr(currentPoint, slice.length);
		newToken.lineNumber = c_lineNumber;

		recognize(newToken, slice.mode);

		if (newToken.tokenState == TokenState::word && tokens.size() == 0)
			throw LexerError("cannot recognize the instruction " + newToken.stringValue, newToken.lineNumber);

		currentPoint += slice.length;
	
		tokens.push_back(newToken);
	}
}


void Tokenizer::recognize(Token& token, const uint8_t& c_mode)
{
	if (c_mode == c_WORD)
	{
		if (token.stringValue[token.stringValue.size() - 1] == ':')
		{
			token.tokenState = TokenState::label;
			token.stringValue.pop_back();
		}
		else
			recognizeToken(token, c_stringInstructions, TokenState::op_add);
	}
	else if (c_mode == c_NUMBER)
		token.tokenState = TokenState::number;
}


void Tokenizer::recognizeToken(Token& token, const std::vector<std::string>& c_array, const TokenState c_tokenState)
{
	for (unsigned i = 0; i < c_array.size(); ++i)
	{
		if (token.stringValue == c_array[i])
		{
			token.tokenState = static_cast<TokenState>(static_cast<int>(c_tokenState) + i);
			return;
		}
	}

	token.tokenState = TokenState::word;
}


Tokenizer::StringSlice& Tokenizer::getSlice(const std::string& c_line, const unsigned& c_startPoint, StringSlice& slice)
{
	if (isalpha(c_line[c_startPoint]))
		slice.mode = c_WORD;
	else if (isdigit(c_line[c_startPoint]) || c_line[c_startPoint] == '-')
		slice.mode = c_NUMBER;
	else
	{
		bool isPunctuationSymbol = std::any_of(c_punctuationSymbols.begin(), c_punctuationSymbols.end(), [&](const auto& symbol) { return symbol[0] == c_line[c_startPoint]; });
		return (isPunctuationSymbol ? (slice = {1, c_PUNCTUATIONSYMBOL }) : (slice = {0, c_PUNCTUATIONSYMBOL}));
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