#include "parser.hpp"

void Parser::parse(const std::vector<Token>& tokens)
{
	int8_t insturctionValue;
	uint8_t instruction;

	// Fix: I think it could be removed
	bool needValues = false;

	for (auto& i : tokens)
	{
		if (i.tokenState <= TokenState::op_hlt)
		{
			assert(needValues == false);

			instruction = static_cast<uint8_t>(i.tokenState);
			instructions.push_back(instruction);

			needValues = insturctionValue = c_instructionValues[instruction];

			if (i.tokenState >= TokenState::op_jmp && i.tokenState <= TokenState::op_je)
			{
				addLocationOfJump(i.stringValue, (instructions.size() - 1));
				instructions.push_back((uint8_t)0);
			}
		}
		else if (i.tokenState == TokenState::label)
		{
			addLocationOfLabel(i.stringValue, (instructions.size() - 1));
		}
		else if (i.tokenState == TokenState::number)
		{
			// Fix: strings are also values!
			try { instructions.push_back(std::stoi(i.stringValue)); }
			catch (const std::invalid_argument&) { printErrorAndExit(c_parserError + "the value is invalid"); }
		}
		else // Fix: get rid of exceptions. Write stable code!
			printErrorAndExit(c_parserError + "invalid instruction");
	}
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


auto Parser::findLocationOfJump(const jumpTableList& c_labelIterator, const unsigned& c_locationToFind)
{
	if (c_labelIterator != jumpTable.end())
		for (auto i = c_labelIterator->second.locationOfJump.begin(); i != c_labelIterator->second.locationOfJump.end(); ++i)
			if (*i == c_locationToFind)
				return i;

	return c_labelIterator->second.locationOfJump.end();
}