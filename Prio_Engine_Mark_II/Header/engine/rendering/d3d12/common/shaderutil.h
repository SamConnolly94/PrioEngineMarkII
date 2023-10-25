#pragma once

#include <string>
#include <d3dcommon.h>
#include <wrl.h>


namespace PrioEngine
{
    namespace Rendering
    {
        namespace D3D12
        {
            Microsoft::WRL::ComPtr<ID3D10Blob> CompileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);
        }
    }
}