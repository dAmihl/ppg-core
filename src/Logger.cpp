#include "Logger.h"

namespace PPG
{
	void Logger::log(const std::string msg)
	{
		Logger::log(msg, 0);
	}

	void Logger::log(const std::string msg, unsigned int logLevel)
	{
		if (logLevel < MINIMUM_LOG_LEVEL) {
			return;
		}

		std::time_t currTime = std::time(nullptr);

		std::ofstream file("ppg-core.log", std::ofstream::app);
		file << std::asctime(std::localtime(&currTime)) << ": " << msg << "\n";
		file.close();
	}

	void Logger::error(const std::string msg)
	{
		Logger::log(msg, 3);
	}

	void Logger::warning(const std::string msg)
	{
		Logger::log(msg, 2);
	}

	void Logger::info(const std::string msg)
	{
		Logger::log(msg, 1);
	}

}

