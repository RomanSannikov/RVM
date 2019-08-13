#include "parser.hpp"

void Parser::parse(const std::vector<Token>& tokens)
{
	uint8_t instructionValuePattern = 0b11110000;
	uint8_t instructionValue = c_instructionValues[static_cast<uint8_t>(tokens[0].tokenState)];
	auto it_currentToken = tokens.begin();

	for (; it_currentToken != tokens.end(); ++it_currentToken)
	{ if (it_currentToken->tokenState < TokenState::space) break; }

	if (it_currentToken->tokenState == TokenState::label)
		addLocationOfLabel(it_currentToken->stringValue, (instructions.size() - 1));
	else if (it_currentToken->tokenState <= TokenState::op_hlt)
		makeInstruction(*it_currentToken);
	else	
		printErrorAndExit(c_parserError + "cannot make an instruction");


	for (; it_currentToken != tokens.end(); ++it_currentToken)
	{
		if (it_currentToken->tokenState == TokenState::word || it_currentToken->tokenState == TokenState::number)
		{
			if (instructionValue & instructionValuePattern)
				makeValue(*it_currentToken);
			else
				printErrorAndExit(c_parserError + "too many arguments");

			instructionValuePattern >>= 4;
		}
	}

	// Todo: test for instruction completeness
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

	if (findLocationOfJump(c_it_jumpTableNode, c_locationOfJump) != jumpTableNode.locationOfJump.end())
		printErrorAndExit(c_parserError + "jump instruction is already exits");
	else if (c_it_jumpTableNode == jumpTable.end())
	{
		jumpTableNode.locationOfLabel = 0;
		jumpTableNode.locationOfJump.push_back(c_locationOfJump);
		jumpTable.insert(std::make_pair(c_labelName, jumpTableNode));
	}
	else
		jumpTableNode.locationOfJump.push_back(c_locationOfJump);

}


void Parser::makeInstruction(const Token& token)
{
	instructions.push_back(static_cast<uint8_t>(token.tokenState));

	if (token.tokenState >= TokenState::op_jmp && token.tokenState <= TokenState::op_je)
	{
		addLocationOfJump(token.stringValue, (instructions.size() - 1));
		instructions.push_back((uint8_t)0);
	}
}


void Parser::makeValue(const Token& token)
{
	if (token.tokenState == TokenState::number)
		instructions.push_back(std::stoi(token.stringValue));
	// Todo: add string support
}