#include "parser.hpp"


void Parser::parse(const std::vector<Token>& c_tokens)
{
	uint8_t instructionValue = 0;
	auto it_currentToken = c_tokens.begin();
	decltype(it_currentToken) it_lastToken;
	
	if (it_currentToken->tokenState == TokenState::word)
		addLocationOfLabel(it_currentToken->stringValue, (instructions.size() - 1));
	else if (it_currentToken->tokenState <= TokenState::op_hlt)
	{
		instructionValue = c_instructionValues[static_cast<uint8_t>(c_tokens[0].tokenState)];
		instructions.push_back(static_cast<uint8_t>(it_currentToken->tokenState));
	}
	else
		printErrorAndExit(c_parserError + "cannot make an instruction", it_currentToken->lineNumber);

	it_lastToken = it_currentToken;
	++it_currentToken;

	if (instructionValue)
		checkArguments(it_currentToken, it_lastToken, instructionValue, c_tokens.end());

	if (it_currentToken != c_tokens.end())
		printErrorAndExit(c_parserError + "too many arguments", it_currentToken->lineNumber);
}


void Parser::addLocationOfLabel(const std::string& c_labelName, const unsigned&& c_locationLabel)
{
	const auto& c_it_jumpTableNode = jumpTable.find(c_labelName);
	jumpTableNode jumpTableNode;

	if (c_it_jumpTableNode != jumpTable.end() && c_it_jumpTableNode->second.locationOfLabel >= 0)
		printErrorAndExit(c_parserError + "The label " + c_it_jumpTableNode->first + " was defined several times!");
	else if (c_it_jumpTableNode == jumpTable.end())
	{
		jumpTableNode.locationOfLabel = c_locationLabel;
		jumpTable.insert(std::make_pair(c_labelName, jumpTableNode));
	}
	else
		c_it_jumpTableNode->second.locationOfLabel = c_locationLabel;
}


auto Parser::findLocationOfJump(const jumpTableList& c_labelIterator, const unsigned& c_locationToFind)
{
	if (c_labelIterator != jumpTable.end())
		for (auto i = c_labelIterator->second.locationOfJump.begin(); i != c_labelIterator->second.locationOfJump.end(); ++i)
			if (*i == c_locationToFind)
				return i;

	return c_labelIterator->second.locationOfJump.end();
}


void Parser::addLocationOfJump(const std::string& c_labelName, const unsigned&& c_locationOfJump)
{
	const auto& c_it_jumpTableNode = jumpTable.find(c_labelName);
	jumpTableNode jumpTableNode;

	if (c_it_jumpTableNode == jumpTable.end())
	{
		jumpTableNode.locationOfLabel = 0;
		jumpTableNode.locationOfJump.push_back(c_locationOfJump);
		jumpTable.insert(std::make_pair(c_labelName, jumpTableNode));
	}
	else if (findLocationOfJump(c_it_jumpTableNode, c_locationOfJump) != c_it_jumpTableNode->second.locationOfJump.end())
		printErrorAndExit(c_parserError + "jump instruction is already exits");
	else
		jumpTableNode.locationOfJump.push_back(c_locationOfJump);

}


void Parser::checkArguments(std::vector<Token>::const_iterator& it_currentToken, 
		std::vector<Token>::const_iterator& it_lastToken, const uint8_t& c_instructionValue, const std::vector<Token>::const_iterator c_it_tokenEnd)
{
	uint8_t instructionValuePattern = 0b11110000;
	uint8_t numberOfIteration = 0;

	for (uint8_t i = 0b11110000; i != 0; i >>= 4)
		if (c_instructionValue & i)
			++numberOfIteration;
	
	for (; numberOfIteration > 0; --numberOfIteration, instructionValuePattern >>= 4, ++it_currentToken)
	{
		if (it_currentToken == c_it_tokenEnd)
			printErrorAndExit(c_parserError + "too few arguments", it_currentToken->lineNumber);

		if (c_instructionValue & instructionValuePattern)
		{
			if (it_lastToken->tokenState >= TokenState::op_jmp &&
				it_lastToken->tokenState <= TokenState::op_je)
			{
				addLocationOfJump(it_currentToken->stringValue, instructions.size() - 1);
				instructions.push_back(0);
			}
			else
				makeValue(*it_currentToken);
			// Todo: the makeValue function doesn't work for loading and saving
		}

		it_lastToken = it_currentToken;
	}
}


void Parser::makeValue(const Token& c_token)
{
	if (c_token.tokenState == TokenState::number)
		instructions.push_back(std::stoi(c_token.stringValue));
	else if (c_token.tokenState == TokenState::word)
	{
		for (auto& i : c_token.stringValue)
			instructions.push_back(i);
		instructions.push_back(0);
	}
	else
		printErrorAndExit(c_parserError + "not an argument", c_token.lineNumber);
}