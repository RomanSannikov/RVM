#pragma once

#include <vector>

#include "instruction.hpp"

class Tokenizer
{
public:
	std::vector<Token> tokens;

public:
	Tokenizer() {}
	

	void tokenize(std::string&);

private:
	Token& match(std::string);
};

