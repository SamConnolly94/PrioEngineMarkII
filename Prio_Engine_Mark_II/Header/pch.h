#pragma once

// add headers that you want to pre-compile here
#include <framework.h>
#include <exception>

namespace PrioEngine
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }
}