#pragma once

#include <memory>
#include <vector>

#include "utils.hpp"

class StackFrameManager
{
private:
	struct StackFrame
	{
		uint16_t programPointer = 0;
		std::vector<stackType> variables;
	};

	std::vector<StackFrame> frames;

public:
	StackFrameManager() = default;

	void pushFrame(uint16_t newProgramPointer = 0) { frames.push_back({.programPointer = newProgramPointer}); }
	void popFrame() { frames.pop_back(); }
	void incProgramPointer() { ++frames.back().programPointer; }
	uint16_t getProgramPointer() { return frames.back().programPointer; }
	void setProgramPointer(uint16_t new_value) { frames.back().programPointer = new_value; }
	void addVariable(stackType reference) { frames.back().variables.push_back(reference); }
	stackType getVariable(stackType variableNumber) { return frames.back().variables.at(variableNumber); }

	std::vector<StackFrame>::iterator begin() { return frames.begin(); }
	std::vector<StackFrame>::iterator end() { return frames.end(); }
};
