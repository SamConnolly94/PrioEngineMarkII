#pragma once
#include <exception>

namespace PrioEngineII
{
    struct EngineCreationException : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "An exception occured when creating the engine.";
        }
    };
}