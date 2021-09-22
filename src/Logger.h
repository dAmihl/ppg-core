#pragma once

#include "PuzzGenCore.h"
#include <iostream>
#include <fstream>
#include <ctime>

namespace PPG
{
	class PPG_EXPORT Logger {

	public:
		static void log(const std::string msg);
		static void log(const std::string msg, unsigned int logLevel);
		static void error(const std::string msg);
		static void warning(const std::string msg);
		static void info(const std::string msg);

	private:
		static unsigned const int MINIMUM_LOG_LEVEL = 3;
	};
}

