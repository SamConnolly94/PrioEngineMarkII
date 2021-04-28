#include "pch.h"
#include "Logger.h"

// External library includes
#include <chrono>
#include <date.h>
#include <iostream>
#include <fstream>
#include <magic_enum.hpp>
#include <Windows.h>

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

void Logger::Write(std::string message, PrioEngineII::ELogVerbosity logVerbosity)
{
	// Create the message from what was passed in
	std::string logMessage = ConstructLogMessage(message, logVerbosity);

	// Write message to the log file
	WriteToFile(logMessage);

	// Write message to output window
	WriteMessageToOutputWindow(logMessage);
}

std::string Logger::ConstructLogMessage(std::string& message, ELogVerbosity& logVerbosity)
{
	// Evaluate the verbosity statement on the log message
	auto verbosityString = magic_enum::enum_name(logVerbosity);

	// Evaluate the timestamp on the log message
	auto timestamp = date::format("%H:%M", chrono::system_clock::now());

	// Format the log message that should be output
	std::string logMessage = "[" + static_cast<std::string>(verbosityString) + "] " + timestamp + ": " + message;

	return logMessage;
}

void Logger::WriteToFile(std::string& logMessage)
{
	// Open the log file for writing
	ofstream logFile(mDirectory + mFilename);

	if (logFile.is_open())
	{
		logFile << logMessage << std::endl;
		logFile.close();
	}
}

void Logger::WriteMessageToOutputWindow(std::string& logMessage)
{
	// Format as a wide string to be output to console
	std::wstring wideStrLogMessage(logMessage.begin(), logMessage.end());

	// Output to console
	OutputDebugString(wideStrLogMessage.c_str());
}