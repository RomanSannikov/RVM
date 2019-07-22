#pragma once

#include <array>
#include <algorithm>
#include <iostream> // FIX: temp
#include <cstring>

#include "instruction.hpp"

class Tokenizer
{
public:
	std::vector<Token> tokens;

private:
	const std::array<char, 3> c_punctuationSymbols;

	const uint8_t c_WORD;
	const uint8_t c_NUMBER;
	const uint8_t c_PUNCTUATIONSYMBOL;

	struct StringSlice
	{
		int length;
		uint8_t mode;
	};

public:
	Tokenizer() : c_punctuationSymbols{{' ', ',', '"'}}, c_WORD(0), c_NUMBER(1), c_PUNCTUATIONSYMBOL(2) {}
	

	void tokenize(std::string&);

private:
	Token recognize(const std::string&);

	StringSlice& is_(const std::string&, const int&);
};

