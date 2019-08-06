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

			// Todo: jump instruction branch

			thenValuesGo = true;
		}
		else if (i.tokenState == TokenState::label)
		{
			const auto& it_jumpTableNode = jumpTable.find(i.stringValue);
			jumpTableNode jumpTableNode;
			uintptr_t locationOfLabel;

			// the location of the last instruction
			locationOfLabel = reinterpret_cast<uintptr_t>(instructions.data() + instructions.size() - 1);
			
			if(it_jumpTableNode != jumpTable.end() && it_jumpTableNode->second.locationOfLabel != 0)
				printErrorAndExit(c_parserError + "The label " + it_jumpTableNode->first + " was defined several times!");
			else if (it_jumpTableNode == jumpTable.end())
			{
				jumpTableNode.locationOfJump = 0;
				jumpTableNode.locationOfLabel = locationOfLabel;

				jumpTable.insert(std::make_pair(i.stringValue, jumpTableNode));
			}
			else
				it_jumpTableNode->second.locationOfLabel = locationOfLabel;
				
			thenValuesGo = true;
		}
	}
}