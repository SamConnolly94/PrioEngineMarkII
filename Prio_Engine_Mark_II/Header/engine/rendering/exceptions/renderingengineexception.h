#pragma once

#include <exception>

struct RenderingEngineException : public std::exception
{
    const char* what() const throw()
    {
        return "A rendering engine exception has occurred.";
    }
};

struct RenderingEngineNotSupported : public std::exception
{
    const char* what() const throw()
    {
        return "The rendering engine specified is not currently supported.";
    }
};