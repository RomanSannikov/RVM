#include "loggingSystem.hpp"

void LoggingSystem::run()
{
	while (true)
	{
		mutex.lock();
		
		if (!messageQueue.empty())
		{
			
			for (ErrorMessage& i : messageQueue)
			{
				handle(i);
			}

		}

		mutex.unlock();
	}
}


void LoggingSystem::handle(ErrorMessage& message)
{
	std::cout << std::endl << "MESSAGE STATUS: ";
	
	if (message.messageState == ErrorMessageState::Yello)
		std::cout << "YELLOW";
	else
		std::cout << "RED";

	std::cout << std::endl << "MESSAGE: " << message.message << std::endl;
}