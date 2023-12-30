#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <bitset> // Fix: just for tests
#include <list>

#include "instruction.hpp"
#include "error.hpp"
#include "logging.hpp"

class Parser
{
public:
	struct jumpTableNode
	{
		uint16_t locationOfLabel;
		std::vector<uint16_t> locationsOfJumps;
	};

	using jumpTableListIter = std::unordered_map<std::string, jumpTableNode>::iterator;

private:
	std::unordered_map<std::string, uint8_t> symbolTable;
	std::unordered_map<std::string, jumpTableNode> jumpTable;
	std::vector<std::string> labelNames;
	std::vector<int8_t> instructions;

	bool wasHlt;

public: 
	Parser() : wasHlt(false) {}

public:
	void parse(const std::vector<Token>&);
	void outputInstructions(std::string);
	void completeParsing();
	constexpr std::vector<int8_t>& getInstructions() { return instructions; }
	inline void loadInstructions(const std::vector<int8_t>& c_givenInstructions) { instructions = std::move(c_givenInstructions); }

private:
	void completeJumpInstructions();
	void checkSymbolTabel();
	void addLocationOfLabel(const std::string&, const uint16_t&&);
	void addLocationOfJump(const std::string&, const uint16_t &&);
	auto findLocationOfJump(const jumpTableListIter&, const unsigned&);
	void checkArguments(std::vector<Token>::const_iterator&, std::vector<Token>::const_iterator&, const uint8_t&, std::vector<Token>::const_iterator);
	void makeValue(const Token&);
};