#pragma once
#include "pch.h"
#include "maths.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <cstdint>

namespace PrioEngine
{
    class Maths
    {
        public:

            template <typename T>
            static T __declspec (dllexport) Clamp<T>(const T& value, const T& min, const T& max)
            {
                return value < low ? low : (value > high ? high : value);
            }

            template <typename T>
            static T __declspec (dllexport) Min<T>(const T& value, const T& min)
            {
                return value < min ? min : value;
            }
    };
}