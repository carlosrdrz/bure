#pragma once

#include <string>

class Error
{
	public:
		static void Log(std::string error, int import);
		static void Log(int error, int import);
};
