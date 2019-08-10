#include "parser.hpp"


void Parser::parse(std::vector<Token>& tokens)
{
	int8_t insturctionValue;
	uint8_t instruction;

	bool thenValuesGo = false;

	for (auto& i : tokens)
	{
		if (i.tokenState <= TokenState::op_hlt)
		{
			assert(thenValuesGo == false);

			instruction = static_cast<uint8_t>(i.tokenState);
			instructions.push_back(instruction);

			insturctionValue = c_instructionValues[instruction];

			// Fix: finish this branch up
			if (i.tokenState >= TokenState::op_jmp && i.tokenState <= TokenState::op_je)
			{
				addLocationOfJump(i.stringValue, reinterpret_cast<intptr_t>(instructions.data() + instructions.size() - 1));
				instructions.push_back((uint8_t)0);
			}

			thenValuesGo = true;
		}
		else if (i.tokenState == TokenState::label)
		{
			addLocationOfLabel(i.stringValue, reinterpret_cast<intptr_t>(instructions.data() + instructions.size() - 1));

			thenValuesGo = true;
		}
	}
}


void Parser::addLocationOfLabel(const std::string& c_labelName, const intptr_t&& c_location)
{
	const auto& c_it_jumpTableNode = jumpTable.find(c_labelName);
	jumpTableNode jumpTableNode;

	if (c_it_jumpTableNode != jumpTable.end() && c_it_jumpTableNode->second.locationOfLabel >= 0)
		printErrorAndExit(c_parserError + "The label " + c_it_jumpTableNode->first + " was defined several times!");
	else if (c_it_jumpTableNode == jumpTable.end())
	{
		// jumpTableNode.locationOfJump = 0;
		jumpTableNode.locationOfLabel = c_location;

		jumpTable.insert(std::make_pair(c_labelName, jumpTableNode));
	}
	else
		c_it_jumpTableNode->second.locationOfLabel = c_location;
}


void Parser::addLocationOfJump(const std::string& c_labelName, const intptr_t&& c_location)
{
	const auto& c_it_jumpTableNode = jumpTable.find(c_labelName);
	jumpTableNode jumpTableNode;

	// Todo: do something with locationOfJump (note that's a vector)
}