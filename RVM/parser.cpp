#include "parser.hpp"


void Parser::parse()
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


void Parser::addLocationOfLabel(std::string& labelName, intptr_t&& location)
{
	const auto& it_jumpTableNode = jumpTable.find(labelName);
	jumpTableNode jumpTableNode;

	if (it_jumpTableNode != jumpTable.end() && it_jumpTableNode->second.locationOfLabel >= 0)
		printErrorAndExit(c_parserError + "The label " + it_jumpTableNode->first + " was defined several times!");
	else if (it_jumpTableNode == jumpTable.end())
	{
		// jumpTableNode.locationOfJump = 0;
		jumpTableNode.locationOfLabel = location;

		jumpTable.insert(std::make_pair(labelName, jumpTableNode));
	}
	else
		it_jumpTableNode->second.locationOfLabel = location;
}


void Parser::addLocationOfJump(std::string& labelName, intptr_t&& location)
{
	const auto& it_jumpTableNode = jumpTable.find(labelName);
	jumpTableNode jumpTableNode;

	// Todo: do something with locationOfJump (note that's a vector)
}