#include "pch.h"
#include "Logger.h"
#include <chrono>
#include <date.h>
#include <iostream>
#include <fstream>
#include <magic_enum.hpp>

using namespace std;
using namespace PrioEngineII;

Logger* Logger::mInstance = nullptr;;

Logger::Logger()
{
	auto dateTime = date::format("%F, %T", chrono::system_clock::now());
	mFilename = "Prio Engine Mark II Log " + dateTime;
}

Logger* Logger::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Logger();
	}

	return mInstance;
}

void Logger::WriteToLog(std::string message, PrioEngineII::ELogVerbosity logVerbosity)
{
	ofstream logFile(mFilename);

	//logFile.open(mFilename);
	if (logFile.is_open())
	{
		auto verbosityString = magic_enum::enum_name(logVerbosity);
		auto timestamp = date::format("%T", chrono::system_clock::now());
		logFile << "[" << verbosityString << "] " << timestamp << ":" << message << std::endl;
		logFile.close();
	}
}
