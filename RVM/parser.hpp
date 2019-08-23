#pragma once

#include <iostream> // Fix: temporary
#include <vector>
#include <unordered_map>
#include <cassert> // Fix: just for tests
#include <string>

#include "instruction.hpp"
#include "error.hpp"

#include <bitset> // Fix: just for tests

class Parser
{
public:
	struct jumpTableNode 
	{ uint16_t locationOfLabel; std::vector<uint16_t> locationsOfJumps; };

	using jumpTableList = std::list<std::pair<const std::string, Parser::jumpTableNode>, std::allocator< std::pair<const std::string, Parser::jumpTableNode>>>::iterator;
	
private:
	std::unordered_map<std::string, uint8_t> symbolTable;
	std::unordered_map<std::string, jumpTableNode> jumpTable;
	std::vector<std::string> labelNames;
	std::vector<uint8_t> instructions;

public: 
	Parser() {}

public:
	void parse(const std::vector<Token>&);
	
	void printInstructions() 
	{ for (auto i : instructions) std::cout << std::bitset<8>(i) << std::endl; }
	
	void completeParsing();

private:
	void completeJumpInstructions();
	void checkSymbolTabel();

	void addLocationOfLabel(const std::string&, const unsigned&&);

	// Todo: add this description to the documentation
	// Desc: the last argument of addLocationOfJump is the location of the jump instruction,
	// therefore when the location of label is added after the instruction (the space is added already)
	void addLocationOfJump(const std::string&, const unsigned&&);

	auto findLocationOfJump(const jumpTableList&, const unsigned&);

	void checkArguments(std::vector<Token>::const_iterator&,
		std::vector<Token>::const_iterator&, const uint8_t&, std::vector<Token>::const_iterator);

	void makeValue(const Token&);
};