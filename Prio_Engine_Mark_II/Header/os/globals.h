#pragma once

#include <Windows.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;
using namespace std;

namespace PrioEngine
{
    namespace Globals
    {
        namespace OS
        {
            HINSTANCE g_HinstDLL;
            HWND g_Hwnd;
        }
    }
}