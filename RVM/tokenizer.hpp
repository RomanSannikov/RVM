#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

#include "instruction.hpp"

class Tokenizer
{
public:
	std::vector<Token> tokens;

public:
	Tokenizer() {}
	

	void tokenize(std::string&);

private:
	Token& recognize(std::string);
};

