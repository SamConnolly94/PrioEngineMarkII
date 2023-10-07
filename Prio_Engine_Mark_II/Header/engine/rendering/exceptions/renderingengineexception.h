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

struct UnexpectedMsaaQualityLevels : public std::exception
{
    const char* what() const throw()
    {
        return "Unexpected Multi Sample Anti Aliasing (MSAA) quality level.";
    }
};

struct DirectXDeviceNotInitialised : public std::exception
{
    const char* what() const throw()
    {
        return "The DirectX device has not been initialised.";
    }
};

struct SwapChainNotInitialised : public std::exception
{
    const char* what() const throw()
    {
        return "The swap chain has not been initialised.";
    }
};

struct DirectXCommandListAllocationNotInitialised : public std::exception
{
    const char* what() const throw()
    {
        return "The DirectX command list allocation has not been initialised.";
    }
};