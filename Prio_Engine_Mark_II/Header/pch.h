#pragma once

// add headers that you want to pre-compile here
#include <framework.h>
#include <exception>
#include <wrl.h>
#include <d3d12.h>
#include <iostream>

namespace PrioEngine
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw std::exception();
        }
    }

    inline void ThrowIfFailed(HRESULT hr, Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice)
    {
        if (FAILED(hr))
        {
            HRESULT reason = m_d3dDevice->GetDeviceRemovedReason();
            std::cout << reason  << std::endl << hr << std::endl;
            throw std::exception();
        }
    }
}