#pragma once

#include <string>
#include "ELogVerbosity.h"

namespace PrioEngineII
{
	/// <summary>
	/// A singleton class which handles the logging messages from the engine
	/// </summary>
	class Logger
	{
	protected:
		/// <summary>
		/// Internal constructor to avoid being called from outside the class
		/// </summary>
		Logger();

		/// <summary>
		/// A reference to the singleton instance
		/// </summary>
		static Logger* mInstance;
	public:
		/// <summary>
		/// Deleted copy constructor to ensure class can operate as singleton
		/// </summary>
		/// <param name="ToCopy">Copy constructor deleted, no parameters are valid.</param>
		Logger(Logger&) = delete;
		
		/// <summary>
		/// Deleted assignment operator to ensure class can operate as singleton
		/// </summary>
		/// <param name="ToAssign">Assignment operator deleted, no parameters are valid.</param>
		void operator=(const Logger&) = delete;

		/// <summary>
		/// A static method which will return the existing instance if it exists, if one does not exist it will initialise one and return the intiailised object.
		/// </summary>
		/// <returns>A reference to the singleton instance of the logger</returns>
		static Logger *GetInstance();
	private:
		/// <summary>
		/// The filename of the log to be written to
		/// </summary>
		std::string mFilename;

		/// <summary>
		/// The directory which logs will be written to
		/// </summary>
		const std::string mDirectory = ".\\Logs\\";
	public:
		/// <summary>
		/// Writes a message to the output window and the log file
		/// </summary>
		/// <param name="message">The message to output</param>
		/// <param name="logVerbosity">The verbosity at which this log should be output</param>
		void Write(std::string message, PrioEngineII::ELogVerbosity logVerbosity);
	private:
		/// <summary>
		/// Creates the message to be output to the log based on the parameters the user provided
		/// </summary>
		/// <param name="message">The message the user wishes to output to the log</param>
		/// <param name="logVerbosity">The verbosity at which the log should mark the message as</param>
		/// <returns>The message that should be output to the log</returns>
		std::string ConstructLogMessage(std::string& message, ELogVerbosity& logVerbosity);

		/// <summary>
		/// Write a message to the log file
		/// </summary>
		/// <param name="logMessage">The message to write to the file</param>
		void WriteToFile(std::string& logMessage);

		/// <summary>
		/// Output a message to the output window
		/// </summary>
		/// <param name="logMessage">The message to output to the output window</param>
		void WriteMessageToOutputWindow(std::string& logMessage);
	};

}