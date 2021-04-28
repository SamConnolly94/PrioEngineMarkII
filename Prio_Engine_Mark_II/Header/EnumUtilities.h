#pragma once

#include "EEngineTypes.h"
#include "ELogVerbosity.h"
#include <string>

namespace PrioEngineII
{
	class EnumUtilities
	{
	private:
		EnumUtilities() = delete;
		EnumUtilities(EnumUtilities&) = delete;
		void operator= (const EnumUtilities&) = delete;
	public:
		static std::string ToString(EEngineTypes engineType);
		static std::string ToString(ELogVerbosity logVerbosity);
	};
}

