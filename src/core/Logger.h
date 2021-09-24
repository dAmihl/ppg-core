#pragma once

#include "Core.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

namespace PPG
{
	class PPG_EXPORT Logger {

	public:
		static void log(const Str msg);
		static void log(const Str msg, unsigned int logLevel);
		static void error(const Str msg);
		static void warning(const Str msg);
		static void info(const Str msg);

	private:
		static unsigned const int MINIMUM_LOG_LEVEL = 3;
	};
}

