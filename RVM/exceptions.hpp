#pragma once

#include <stdexcept>

const std::string c_argumentParserError = "Argument Parser Error: ";
const std::string c_scannerError = "Scanner Error: ";
const std::string c_lexerError = "Lexer Error: ";
const std::string c_parserError = "Parser Error: ";
const std::string c_executerError = "Executer Error: ";

class RVMError : public std::runtime_error
{
public:
	explicit RVMError(const std::string& message) : runtime_error(message) {}
	explicit RVMError(const std::string& message, unsigned lineNumber) : runtime_error(message + "\nline: " + std::to_string(lineNumber)) {}
};
