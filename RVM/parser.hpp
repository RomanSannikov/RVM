#pragma once

#include <iostream> // Fix: temporary
#include <vector>
#include <unordered_map>
#include <cassert> // Test
#include <string>

#include "instruction.hpp"
#include "error.hpp"

class Parser
{
public:
	// Todo: add this description to the documentation
	// Decs: the locations are integers. This means to find 
	// the place in instructions use instructions[locationOf...]
	struct jumpTableNode 
	{ unsigned locationOfLabel; std::vector<unsigned> locationOfJump; };

	using jumpTableList = std::list<std::pair<const std::string, Parser::jumpTableNode>, std::allocator< std::pair<const std::string, Parser::jumpTableNode>>>::iterator;

private:
	// Desc:		   name		    type
	std::unordered_map<std::string, uint8_t> symbolTable;
	std::unordered_map<std::string, jumpTableNode> jumpTable;
	std::vector<uint8_t> instructions;

public: 
	Parser() {}

public:
	void parse(const std::vector<Token>&);
	// Todo: add a function that will complete the jumpTable after all parsing

private:
	void addLocationOfLabel(const std::string&, const unsigned&&);

	// Todo: add this description to the documentation
	// Desc: the last argument of addLocationOfJump is the location of the jump instruction,
	// therefore when the location of label is added after the instruction (the space is added already)
	void addLocationOfJump(const std::string&, const unsigned&&);

	auto findLocationOfJump(const jumpTableList&, const unsigned&);
};