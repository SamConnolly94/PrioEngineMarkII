#include <pch.h>
#include <engine/common/stringconversion.h>

#include <stringapiset.h>

std::wstring PrioEngine::Common::StringToWideString(const std::string& str)
{
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), NULL, 0);
    std::wstring result(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], sizeNeeded);

    // TODO:
    // Consider whether this is better placed to be returned as an RValue, gut instinct says no
    // as there's a very solid chance it's quicker to calculate this once, and leave it in place on the stack
    // rather than returning the mem ref to it which is going to be located elsewhere.
    return result;
}