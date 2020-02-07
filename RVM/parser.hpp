#pragma once

#include <iostream> // Fix: temporary
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <bitset> // Fix: just for tests

#include "instruction.hpp"
#include "error.hpp"


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
	std::vector<int8_t> instructions;

	bool wasHlt;

public: 
	Parser() : wasHlt(false) {}

public:
	void parse(const std::vector<Token>&) noexcept;
	
	void printInstructions() noexcept
	{ for (const auto& i : instructions) std::cout << std::bitset<8>(i) << std::endl; }
	
	void outputInstructions(std::string);

	void completeParsing() noexcept;

	constexpr std::vector<int8_t>& getInstructions() noexcept { return instructions; }

	inline void loadInstructions(const std::vector<int8_t>& c_givenInstructions) { instructions = std::move(c_givenInstructions); }

private:
	void completeJumpInstructions() noexcept;
	void checkSymbolTabel() noexcept;

	void addLocationOfLabel(const std::string&, const uint16_t&&) noexcept;

	void addLocationOfJump(const std::string&, const uint16_t &&) noexcept;

	auto findLocationOfJump(const jumpTableList&, const unsigned&) noexcept;

	void checkArguments(std::vector<Token>::const_iterator&,
		std::vector<Token>::const_iterator&, const uint8_t&, std::vector<Token>::const_iterator) noexcept;

	void makeValue(const Token&) noexcept;
};