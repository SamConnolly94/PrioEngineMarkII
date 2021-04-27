#pragma once

#include <string>
#include "ELogVerbosity.h"

namespace PrioEngineII
{
	class Logger
	{
	protected:
		Logger();
		static Logger* mInstance;
	public:
		Logger(Logger&) = delete;
		void operator=(const Logger&) = delete;
		static Logger *GetInstance();
	private:
		std::string mFilename;
	public:
		void WriteToLog(std::string message, PrioEngineII::ELogVerbosity logVerbosity);
		

	};

}