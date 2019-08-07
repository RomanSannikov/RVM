#pragma once

#include <iostream> // Fix: temporary
#include <vector>
#include <unordered_map>
#include <cassert> // Test

#include "instruction.hpp"
#include "error.hpp"

class Parser
{
private:
	struct jumpTableNode 
	{ intptr_t locationOfLabel; std::vector<intptr_t> locationOfJump; };

	std::unordered_map<std::string, uint8_t> symbolTable; // Fix: Think of type
	std::unordered_map<std::string, jumpTableNode> jumpTable;
	std::vector<uint8_t> instructions;

	std::vector<Token>& tokens;

	unsigned currentLocation;

public: 
	Parser() : currentLocation(0) {}

public:
	void parse();
	// Todo: add a function that will complete the jumpTable after all parsing

private:
	void addLocationOfLabel(std::string&, intptr_t&&);

	// Todo: add this description to the documentation
	// Desc: the last argument of addLocationOfJump is the location of the jump instruction,
	// therefore when the location of label is added after the instruction (the space is added already)
	void addLocationOfJump(std::string&, intptr_t&&);

};