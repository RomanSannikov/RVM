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

	const uint8_t c_WORD;
	const uint8_t c_NUMBER;
	const uint8_t c_PUNCTUATIONSYMBOL;

	struct StringSlice
	{
		unsigned length;
		uint8_t mode;
	};

public:
	Tokenizer() : c_punctuationSymbols{{" ", ",", "\""}}, c_WORD(0), c_NUMBER(1), c_PUNCTUATIONSYMBOL(2) {}
	
	void tokenize(const std::string&);

private:
	void recognize(Token&, const uint8_t&);
	void recognizeToken(Token&, const std::vector<std::string>&, const TokenState);

	StringSlice& getSlice(const std::string&, const unsigned&, StringSlice&);
};

