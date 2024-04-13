#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "exceptions.hpp"


class RVMError : public std::runtime_error
{
public:
	explicit RVMError(const std::string& message) : runtime_error(message) {}
	explicit RVMError(const std::string& message, unsigned lineNumber) : runtime_error(message + "\nline: " + std::to_string(lineNumber)) {}
};
