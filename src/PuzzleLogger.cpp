#include "PuzzleLogger.h"

void PuzzleLogger::log(std::string msg)
{
	PuzzleLogger::log(msg, 0);
}

void PuzzleLogger::log(std::string msg, unsigned int logLevel)
{
	if (logLevel < MINIMUM_LOG_LEVEL) {
		return;
	}

	std::time_t currTime = std::time(nullptr);

	std::ofstream file("ppg-core.log", std::ofstream::app);
	file << std::asctime(std::localtime(&currTime)) << ": " << msg << "\n";
	file.close();
}

void PuzzleLogger::error(std::string msg)
{
	PuzzleLogger::log(msg, 3);
}

void PuzzleLogger::warning(std::string msg)
{
	PuzzleLogger::log(msg, 2);
}

void PuzzleLogger::info(std::string msg)
{
	PuzzleLogger::log(msg, 1);
}
