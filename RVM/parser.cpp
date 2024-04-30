#include "parser.hpp"


void Parser::parseFromFile(const std::filesystem::path& c_filename, bool in_binary)
{
	Scanner scanner(c_filename);
	Tokenizer tokenizer;

	if (in_binary) {
		loadInstructions(scanner.readBinary());
	} else {
		std::string lineFromScanner;

		while (!scanner.isEOF())
		{
			scanner.getLine(lineFromScanner);

			if (lineFromScanner.empty())
				continue;

			Logger::print(lineFromScanner);

			tokenizer.tokenize(lineFromScanner, scanner.getLineNumber());

			parse(tokenizer.tokens);
		}

		completeParsing();
		Logger::printInstructions(getInstructions());
	}
}


void Parser::parse(const std::vector<Token>& c_tokens)
{
	instructionType instructionValue = 0;
	auto it_currentToken = c_tokens.begin();
	decltype(it_currentToken) it_lastToken;
	
	if (c_tokens.size() == 0)
		return;

	if (it_currentToken->tokenState == TokenState::label)
		addLocationOfLabel(it_currentToken->stringValue, (uint16_t)instructions.size());
	else if (it_currentToken->tokenState <= TokenState::op_hlt)
	{
		if (it_currentToken->tokenState == TokenState::op_hlt)
			wasHlt = true;
		instructionValue = c_instructionValues[static_cast<instructionType>(c_tokens.front().tokenState)];
		instructions.push_back(static_cast<instructionType>(it_currentToken->tokenState));
	}
	else
		throw ParserError("cannot make an instruction", it_currentToken->lineNumber);

	it_lastToken = it_currentToken;
	++it_currentToken;

	if (instructionValue)
		checkArguments(it_currentToken, it_lastToken, instructionValue, c_tokens.end());

	if (it_currentToken != c_tokens.end())
		throw ParserError("too many arguments", it_currentToken->lineNumber);
}


void Parser::completeParsing()
{
	completeJumpInstructions();

	if (!wasHlt)
		throw ParserError("There is no HLT instruction. It's an undefined behavior without the instructions");
}


void Parser::completeJumpInstructions()
{
	for (const auto& i : labelNames)
	{
		const auto&& c_it_jumpTableNode = jumpTable.find(i);

		if (c_it_jumpTableNode != jumpTable.end() && c_it_jumpTableNode->second.locationsOfJumps.size() != 0)
		{
			for (const auto& c_locationOfJump : c_it_jumpTableNode->second.locationsOfJumps)
			{
				instructionType* pointerToInstructions = instructions.data();
				pointerToInstructions[c_locationOfJump + 1] = c_it_jumpTableNode->second.locationOfLabel >> 8;
				pointerToInstructions[c_locationOfJump + 2] = (instructionType)c_it_jumpTableNode->second.locationOfLabel;
			}
		}
		else 
			throw ParserError("label " + i + " hasn't been found");
	}
}


void Parser::addLocationOfLabel(const std::string& c_labelName, const uint16_t&& c_locationLabel)
{
	const auto& c_it_jumpTableNode = jumpTable.find(c_labelName);
	jumpTableNode jumpTableNode;

	if (c_it_jumpTableNode != jumpTable.end() && c_it_jumpTableNode->second.locationsOfJumps.size() == 0)
		throw ParserError("The label " + c_it_jumpTableNode->first + " was defined several times!");
	else if (c_it_jumpTableNode == jumpTable.end())
	{
		jumpTableNode.locationOfLabel = c_locationLabel;
		jumpTable.insert(std::make_pair(c_labelName, jumpTableNode));
		labelNames.push_back(c_labelName);
	}
	else
		c_it_jumpTableNode->second.locationOfLabel = c_locationLabel;

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
	else if (std::find(c_it_jumpTableNode->second.locationsOfJumps.begin(), c_it_jumpTableNode->second.locationsOfJumps.end(), c_locationOfJump)
			 != c_it_jumpTableNode->second.locationsOfJumps.end())
		throw ParserError("jump instruction is already exits");
	else
		c_it_jumpTableNode->second.locationsOfJumps.push_back(c_locationOfJump);

}


void Parser::checkArguments(std::vector<Token>::const_iterator& it_currentToken, 
		std::vector<Token>::const_iterator& it_currentInstruction, const uint8_t& c_instructionValue, const std::vector<Token>::const_iterator c_it_tokenEnd)
{
	uint8_t instructionValuePattern = 0b11110000;
	uint8_t numberOfIterations = 0;
	std::array<TokenState, 2> valueTypes;

	for (uint8_t i = 0b11110000; i != 0; i >>= 4)
		if (c_instructionValue & i)
			++numberOfIterations;

	// Desc: Finding out what value types are
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
			throw ParserError("too few arguments", it_currentToken->lineNumber);
		else if (valueTypes[i] != it_currentToken->tokenState)
			throw ParserError("the value isn't the correct type", it_currentToken->lineNumber);

		if (c_instructionValue & instructionValuePattern)
		{
			if ((it_currentInstruction->tokenState >= TokenState::op_jmp &&
				it_currentInstruction->tokenState <= TokenState::op_jnz) || it_currentInstruction->tokenState == TokenState::op_call)
			{
				addLocationOfJump(it_currentToken->stringValue, (uint16_t)(instructions.size() - 1));
				// Desc: locationOfLabel is 2 byte long
				//		  because the instruction vector is of uint8_t and location is of uint16_t
				instructions.push_back(0); instructions.push_back(0);
			}
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
		catch (const std::invalid_argument& invalidArgument) { throw ParserError("the value must be a number", c_token.lineNumber); }
	}
	else if (c_token.tokenState == TokenState::word)
	{
		std::copy(c_token.stringValue.begin(), c_token.stringValue.end(), instructions.begin());
		instructions.push_back(0);
	}
	else
		throw ParserError("not an argument", c_token.lineNumber);
}


void Parser::outputInstructions(std::filesystem::path filename)
{
	filename.replace_extension("rbc");

	std::ofstream output(filename);
	for (const auto& i : instructions) output << std::bitset<8>(i);
	std::cout << "Written to " << filename.generic_string() << std::endl;
	output.close();
}
