#include "Logger.h"

namespace PPG
{
	void Logger::log(const Str msg)
	{
		Logger::log(msg, 0);
	}

	void Logger::log(const Str msg, unsigned int logLevel)
	{
		if (logLevel < MINIMUM_LOG_LEVEL) {
			return;
		}

		auto currTime = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(currTime);
		std::ofstream file("ppg-core.log", std::ofstream::app);
		file << std::ctime(&t) << ": " << msg << "\n";
		file.close();
	}

	void Logger::error(const Str msg)
	{
		Logger::log(msg, 3);
	}

	void Logger::warning(const Str msg)
	{
		Logger::log(msg, 2);
	}

	void Logger::info(const Str msg)
	{
		Logger::log(msg, 1);
	}

}

