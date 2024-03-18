#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <bitset> // Fix: just for tests
#include <list>

#include "instruction.hpp"
#include "scanner.hpp"
#include "tokenizer.hpp"
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
	std::unordered_map<std::string, jumpTableNode> jumpTable;
	std::vector<std::string> labelNames;
	std::vector<instructionType> instructions;

	bool wasHlt;

public: 
	Parser() : wasHlt(false) {}

public:
	void parseFromFile(const std::string&, bool);
	void outputInstructions(std::string);
	constexpr std::vector<instructionType>& getInstructions() { return instructions; }

private:
	void parse(const std::vector<Token>&);
	void completeParsing();
	void completeJumpInstructions();
	void addLocationOfLabel(const std::string&, const uint16_t&&);
	void addLocationOfJump(const std::string&, const uint16_t &&);
	auto findLocationOfJump(const jumpTableListIter&, const unsigned&);
	void checkArguments(std::vector<Token>::const_iterator&, std::vector<Token>::const_iterator&, const uint8_t&, std::vector<Token>::const_iterator);
	void makeValue(const Token&);
	inline void loadInstructions(const std::vector<instructionType>& c_givenInstructions) { instructions = std::move(c_givenInstructions); }
};