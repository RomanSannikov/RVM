#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <iostream>
#include <string>

enum class ErrorMessageState
{
	Yello = 0,
	Red
};

struct ErrorMessage
{
	ErrorMessageState messageState;
	std::string	message;
};

static std::vector<ErrorMessage> messageQueue;
static std::mutex mutex;

inline void commitErrorMessage(ErrorMessage message)
{
	mutex.lock();
	messageQueue.push_back(message);
	mutex.unlock();
}


class LoggingSystem
{
private:
	std::thread loggingSystemThread;

public:
	LoggingSystem() { loggingSystemThread = std::thread(&LoggingSystem::run); }

public:
	void run();

private:
	void handle(ErrorMessage&);
};