#include "pch.h"
#include "Logger.h"
#include <chrono>
#include <date.h>
#include <iostream>
#include <fstream>
#include <magic_enum.hpp>
#include <locale>
#include <codecvt>
#include <Windows.h>
#include <cstdlib>

using namespace std;
using namespace PrioEngineII;

Logger* Logger::mInstance = nullptr;;

Logger::Logger()
{
	auto dateTime = date::format("%F_%H-%M", chrono::system_clock::now());
	mFilename = dateTime + "_Prio-Engine-Log.txt";
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
	ofstream logFile;

	// Evaluate the verbosity statement on the log message
	auto verbosityString = magic_enum::enum_name(logVerbosity);

	// Evaluate the timestamp on the log message
	auto timestamp = date::format("%H:%M", chrono::system_clock::now());

	// Format the log message that should be output
	std::string logMessage = "[" + static_cast<std::string>(verbosityString) + "] " + timestamp + ": " + message;

	// processenv.h
	std::string directory = ".\\Logs\\";
	logFile.open(directory + mFilename);
	if (logFile.is_open())
	{
		logFile << logMessage << std::endl;
		logFile.close();
	}

	// Output to the console
	std::wstring result(logMessage.begin(), logMessage.end());

	OutputDebugString(result.c_str());
}
