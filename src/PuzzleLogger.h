#pragma once

#include <iostream>
#include <fstream>
#include <ctime>

namespace PPG
{
	class PuzzleLogger {

	public:
		static void log(std::string msg);
		static void log(std::string msg, unsigned int logLevel);
		static void error(std::string msg);
		static void warning(std::string msg);
		static void info(std::string msg);

	private:
		static unsigned const int MINIMUM_LOG_LEVEL = 3;
	};
}

