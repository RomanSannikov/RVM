#pragma once

#include <stdexcept>

class RVMError : public std::runtime_error
{
public:
	explicit RVMError(const std::string& message) : runtime_error(message) {}
	explicit RVMError(const std::string& message, unsigned lineNumber) : runtime_error(message + "\nline: " + std::to_string(lineNumber)) {}
};

class ArgumentParserError : public RVMError
{
public:
	explicit ArgumentParserError(const std::string& message) : RVMError("Argument Parser Error: " + message + "\nRVM -h for help") {}
};

class LexerError : public RVMError
{
public:
	explicit LexerError(const std::string& message) : RVMError("Lexer Error: " + message) {}
	explicit LexerError(const std::string& message, unsigned lineNumber) : RVMError("Lexer Error: " + message + "\nline: " + std::to_string(lineNumber)) {}
};

class ScannerError : public RVMError
{
public:
	explicit ScannerError(const std::string& message) : RVMError("Scanner Error: " + message) {}
};

class ParserError : public RVMError
{
public:
	explicit ParserError(const std::string& message) : RVMError("Parser Error: " + message) {}
	explicit ParserError(const std::string& message, unsigned lineNumber) : RVMError("Parser Error: " + message + "\nline: " + std::to_string(lineNumber)) {}
};

class ExecutorError : public RVMError
{
public:
	explicit ExecutorError(const std::string& message) : RVMError("Executor Error: " + message) {}
};

class GCError : public RVMError
{
public:
	explicit GCError(const std::string& message) : RVMError("GC Error: " + message) {}
};
