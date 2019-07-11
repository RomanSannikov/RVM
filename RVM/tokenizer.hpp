#pragma once

#include <vector>

#include "instruction.hpp"

class Tokenizer
{
private:
	std::vector<Token> tokens;

public:
	Tokenizer();
	

	void tokenize();
};

