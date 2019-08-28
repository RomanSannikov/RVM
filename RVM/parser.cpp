#include "parser.hpp"


void Parser::parse(const std::vector<Token>& c_tokens)
{
	uint8_t instructionValue = 0;
	auto it_currentToken = c_tokens.begin();
	decltype(it_currentToken) it_lastToken;
	
	if (it_currentToken->tokenState == TokenState::word)
		addLocationOfLabel(it_currentToken->stringValue, (uint16_t)instructions.size());
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


void Parser::completeParsing()
{
	completeJumpInstructions();
	checkSymbolTabel();
}


void Parser::completeJumpInstructions()
{
	for (const auto& i : labelNames)
	{
		const auto&& c_it_jumpTableNode = jumpTable.find(i);
		
		if (c_it_jumpTableNode != jumpTable.end() || c_it_jumpTableNode->second.locationsOfJumps.size() != 0)
		{
			for (const auto& c_locationOfJump : c_it_jumpTableNode->second.locationsOfJumps)
			{
				int8_t* pointerToInstructions = instructions.data();
				pointerToInstructions[c_locationOfJump + 1] = c_it_jumpTableNode->second.locationOfLabel >> 8;
				pointerToInstructions[c_locationOfJump + 2] = (uint8_t)c_it_jumpTableNode->second.locationOfLabel;
			}
		}
		else 
			printErrorAndExit(c_parserError + "label " + i + " hasn't been found");
	}
}


void Parser::checkSymbolTabel()
{
	if (symbolTable.size() != 0)
		printErrorAndExit(c_parserError + "not all variables were deleted");
}


void Parser::addLocationOfLabel(const std::string& c_labelName, const uint16_t&& c_locationLabel)
{
	const auto& c_it_jumpTableNode = jumpTable.find(c_labelName);
	jumpTableNode jumpTableNode;

	if (c_it_jumpTableNode != jumpTable.end() && c_it_jumpTableNode->second.locationsOfJumps.size() == 0)
		printErrorAndExit(c_parserError + "The label " + c_it_jumpTableNode->first + " was defined several times!");
	else if (c_it_jumpTableNode == jumpTable.end())
	{
		jumpTableNode.locationOfLabel = c_locationLabel;
		jumpTable.insert(std::make_pair(c_labelName, jumpTableNode));
		labelNames.push_back(c_labelName);
	}
	else
		c_it_jumpTableNode->second.locationOfLabel = c_locationLabel;

}


auto Parser::findLocationOfJump(const jumpTableList& c_labelIterator, const unsigned& c_locationToFind)
{
	if (c_labelIterator != jumpTable.end())
		for (auto i = c_labelIterator->second.locationsOfJumps.begin(); i != c_labelIterator->second.locationsOfJumps.end(); ++i)
			if (*i == c_locationToFind)
				return i;

	return c_labelIterator->second.locationsOfJumps.end();
}


void Parser::addLocationOfJump(const std::string& c_labelName, const uint16_t && c_locationOfJump)
{
	const auto& c_it_jumpTableNode = jumpTable.find(c_labelName);
	jumpTableNode jumpTableNode;

	if (c_it_jumpTableNode == jumpTable.end())
	{
		jumpTableNode.locationsOfJumps.push_back(c_locationOfJump);
		jumpTable.insert(std::make_pair(c_labelName, jumpTableNode));
		labelNames.push_back(c_labelName);
	}
	else if (findLocationOfJump(c_it_jumpTableNode, c_locationOfJump) != c_it_jumpTableNode->second.locationsOfJumps.end())
		printErrorAndExit(c_parserError + "jump instruction is already exits");
	else
		c_it_jumpTableNode->second.locationsOfJumps.push_back(c_locationOfJump);

}


void Parser::checkArguments(std::vector<Token>::const_iterator& it_currentToken, 
		std::vector<Token>::const_iterator& it_currentInstruction, const uint8_t& c_instructionValue, const std::vector<Token>::const_iterator c_it_tokenEnd)
{
	uint8_t instructionValuePattern = 0b11110000;
	uint8_t numberOfIterations = 0;
	std::array<TokenState, 2> valueTypes;

	uint8_t variableSize;

	for (uint8_t i = 0b11110000; i != 0; i >>= 4)
		if (c_instructionValue & i)
			++numberOfIterations;
	
	// Desc: Fiding out what value types are
	for (uint8_t i = 0b11000000, index = 0, counter = 4; i != 0; i >>= 2, --counter)
	{
		if (c_instructionValue & i)
		{
			valueTypes[index] = static_cast<TokenState>(static_cast<uint8_t>(TokenState::number) - (counter % 2));
			++index;
		}
	}

	for (int i = 0; numberOfIterations > 0;
		--numberOfIterations, instructionValuePattern >>= 4, ++it_currentToken, ++i)
	{
		if (it_currentToken == c_it_tokenEnd)
			printErrorAndExit(c_parserError + "too few arguments", it_currentToken->lineNumber);
		else if (valueTypes[i] != it_currentToken->tokenState)
			printErrorAndExit(c_parserError + "the value isn't the correct type", it_currentToken->lineNumber);

		if (c_instructionValue & instructionValuePattern)
		{
			if (it_currentInstruction->tokenState >= TokenState::op_jmp &&
				it_currentInstruction->tokenState <= TokenState::op_je)
			{
				addLocationOfJump(it_currentToken->stringValue, (uint16_t)(instructions.size() - 1));
				// Desc: locationOfLabel is 2 byte long
				instructions.push_back(0); instructions.push_back(0);
			}
			else if (it_currentInstruction->tokenState == TokenState::op_new)
			{
				if (valueTypes[i] == TokenState::number)
				{
					try { instructions.push_back(std::stoi(it_currentToken->stringValue)); }
					catch (std::invalid_argument invalidArgument) { printErrorAndExit(c_parserError + "the value must be a number", it_currentToken->lineNumber); }
				}
				else if (symbolTable.find(it_currentToken->stringValue) == symbolTable.end())
				{ 
					symbolTable.insert(std::make_pair(it_currentToken->stringValue, variableSize));
					makeValue(*it_currentToken);
				}
				else
					printErrorAndExit(c_parserError + "the variable " + it_currentToken->stringValue + " has been already defined", it_currentToken->lineNumber);
			}
			else if (it_currentInstruction->tokenState == TokenState::op_del)
			{
				if (symbolTable.erase(it_currentToken->stringValue) == 0)
					printErrorAndExit(c_parserError + "the variable " + it_currentToken->stringValue + " hasn't been defined", it_currentToken->lineNumber);
				makeValue(*it_currentToken);
			}
			else if ((it_currentInstruction->tokenState == TokenState::op_ld || it_currentInstruction->tokenState == TokenState::op_sv) 
																	&& symbolTable.find(it_currentToken->stringValue) == symbolTable.end())
					printErrorAndExit(c_parserError + "the value " + it_currentToken->stringValue + " hasn't been defined", it_currentToken->lineNumber);
			else
				makeValue(*it_currentToken);
		}
	}
}


void Parser::makeValue(const Token& c_token)
{
	if (c_token.tokenState == TokenState::number)
	{
		try { instructions.push_back(std::stoi(c_token.stringValue)); }
		catch (std::invalid_argument invalidArgument) { printErrorAndExit(c_parserError + "the value must be a number", c_token.lineNumber); }
	}
	else if (c_token.tokenState == TokenState::word)
	{
		for (auto& i : c_token.stringValue)
			instructions.push_back(i);
		instructions.push_back(0);
	}
	else
		printErrorAndExit(c_parserError + "not an argument", c_token.lineNumber);
}