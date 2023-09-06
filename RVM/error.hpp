#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "exceptions.hpp"


class RVMError : std::runtime_error
{
public:
	RVMError(std::string message) : runtime_error(message) {}
	RVMError(std::string message, unsigned lineNumber) : runtime_error(message + "\nline: " + std::to_string(lineNumber)) {}
};