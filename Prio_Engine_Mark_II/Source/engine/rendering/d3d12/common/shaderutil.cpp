#include <pch.h>
#include <engine/rendering/d3d12/common/shaderutil.h>

#include <engine/rendering/helper/d3d12util.h>

using Microsoft::WRL::ComPtr;

Microsoft::WRL::ComPtr<ID3D10Blob> PrioEngine::Rendering::D3D12::CompileShader(
    const std::wstring& filename, 
    const D3D_SHADER_MACRO* defines, 
    const std::string& entrypoint, 
    const std::string& target)
{
    UINT compileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = S_OK;

    ComPtr<ID3DBlob> byteCode = nullptr;
    ComPtr<ID3DBlob> errors = nullptr;

    hr = D3DCompileFromFile(
        filename.c_str(), 
        defines, 
        D3D_COMPILE_STANDARD_FILE_INCLUDE, 
        entrypoint.c_str(), 
        target.c_str(), 
        compileFlags, 
        0, 
        &byteCode, 
        &errors);

    if (errors != nullptr)
    {
        ::OutputDebugStringA((char*)errors->GetBufferPointer());
    }

    PrioEngine::ThrowIfFailed(hr);

    return byteCode;
}