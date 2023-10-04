#include <pch.h>
#include <prioengine.h>

#include <dxgi.h>

using namespace DX;

CPrioEngine::CPrioEngine(EGraphicsAPI graphicsApi, unsigned int width, unsigned int height, const char* windowTitle)
{
    switch (graphicsApi)
    {
    case EGraphicsAPI::DX12:
    {
        OnInit();
        // TODO:
        // Init Direct X
        break;
    }
    }
}

void CPrioEngine::OnInit()
{
    LoadPipeline();
    LoadAssets();
}

void CPrioEngine::OnUpdate()
{
}

void CPrioEngine::OnRender()
{
}

void CPrioEngine::OnDestroy()
{
}

void CPrioEngine::LoadPipeline()
{
#ifdef _DEBUG
    // Enable the Direct 3D 12 debug layer.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    if (m_UseWarpDevice)
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

        ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));
    }
    else
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        // TODO: Find how this should be implemented
        GetHardwareAdapter(factory.Get(), &hardwareAdapter);

        // TODO:
        // SHouldn't this really be checking for DX12 support?
        ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));
    }
}

void CPrioEngine::LoadAssets()
{
}

void CPrioEngine::PopulateCommandList()
{
}

void CPrioEngine::WaitForPreviousFrame()
{
}
