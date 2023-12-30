#pragma once

#include <array>
#include <algorithm>
#include <cstring>

#include "instruction.hpp"
#include "error.hpp"

class Tokenizer
{
public:
	std::vector<Token> tokens;

private:
	const std::vector<std::string> c_punctuationSymbols;

	enum : uint8_t
	{ c_WORD = 0, c_NUMBER,	c_PUNCTUATIONSYMBOL };
	
	struct StringSlice
	{
		unsigned length;
		uint8_t mode;
	};

public:
	Tokenizer() : c_punctuationSymbols{{" ", ",", "\""}} {}
	
	void tokenize(const std::string&, const unsigned&);

private:
	void recognize(Token&, const uint8_t&);
	void recognizeToken(Token&, const std::vector<std::string>&, const TokenState);

	StringSlice& getSlice(const std::string&, const unsigned&, StringSlice&);
};

