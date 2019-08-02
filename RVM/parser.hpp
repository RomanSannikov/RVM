#pragma once

#include <iostream> // Fix: temporary
#include <vector>
#include <unordered_map>

#include "error.hpp"

class Parser
{
private:
	struct jumpTableNode 
	{ void* location; int8_t value; };

	std::unordered_map<std::string, uint8_t> symbolTable; // Fix: type
	std::unordered_map<std::string, jumpTableNode> jumpTable;
	std::vector<uint8_t> instructions;

	int currentLocation;
public: 
	Parser() {}

public:
	void parse();

private:
};