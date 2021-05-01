#include "pch.h"
#include "EnumUtilities.h"
#include <magic_enum.hpp>

using namespace PrioEngineII;

std::string EnumUtilities::ToString(EEngineTypes engineType)
{
	return std::string(magic_enum::enum_name(engineType));
}

std::string EnumUtilities::ToString(ELogVerbosity logVerbosity)
{
	return std::string(magic_enum::enum_name(logVerbosity));
}

std::string PrioEngineII::EnumUtilities::ToString(EShaderType shaderType)
{
	return std::string(magic_enum::enum_name(shaderType));
}
