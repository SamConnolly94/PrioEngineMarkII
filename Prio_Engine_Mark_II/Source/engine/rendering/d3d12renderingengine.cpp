#include <pch.h>
#include <engine/rendering/d3d12enderingengine.h>

#include <prioengine.h>
#include <engine/rendering/exceptions/renderingengineexception.h>
#include <engine/rendering/d3d12/common/shaderutil.h>
#include <engine/rendering/d3d12/uploadbuffer.h>

#include <memory>

#ifdef _DEBUG
#include <vector>
#include <string>
#endif

using Microsoft::WRL::ComPtr;
using namespace std;

Microsoft::WRL::ComPtr<ID3D12Device> CD3D12RenderingEngine::GetDevice() const
{
    return m_d3dDevice;
}

bool CD3D12RenderingEngine::Initialise()
{
#if defined(DEBUG) || defined(_DEBUG)
    // Enable the D3D12 Debug Layer
    {
        ComPtr<ID3D12Debug> debugController;
        PrioEngine::ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));  // This isn't actually defined anywhere just yet
        debugController->EnableDebugLayer();
    }
#endif

// Initialise the factory used for creating devices
PrioEngine::ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&m_dxgiFactory)));

// Initialise the device
HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3dDevice));

// Fallback to Windows Advanced Rasterization Platform (WARP) device
if (FAILED(hardwareResult))
{
    ComPtr<IDXGIAdapter> pWarpAdapter;
    PrioEngine::ThrowIfFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

    PrioEngine::ThrowIfFailed(D3D12CreateDevice(
        pWarpAdapter.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&m_d3dDevice)));
}

// TODO:
// What's a fence again in this context? Can't really remember.
PrioEngine::ThrowIfFailed(m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
    IID_PPV_ARGS(&m_Fence)));

m_RtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
m_DsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
m_CbvSrvUavDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

// Checking for 4x MSAA support for back buffer.
// Everything above DX11 should have this.

D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
msQualityLevels.Format = m_BackBufferFormat;
msQualityLevels.SampleCount = 4;
msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
msQualityLevels.NumQualityLevels = 0;
PrioEngine::ThrowIfFailed(m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));

m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
if (m_4xMsaaQuality <= 0)
{
    throw UnexpectedMsaaQualityLevels();
}

#ifdef _DEBUG
LogAdapters();
#endif

CreateCommandObjects();
CreateSwapChain();
CreateRtvAndDsvDescriptorHeaps();

// Do the initial resize code.
OnResize();

return true;
}

void CD3D12RenderingEngine::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));

    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_DirectCmdListAlloc.GetAddressOf())));

    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_DirectCmdListAlloc.Get(),
        nullptr,
        IID_PPV_ARGS(m_CommandList.GetAddressOf())));

    // Command list needs to be closed before being reset
    // So start in a closed state to reset before being used.
    m_CommandList->Close();
}

void CD3D12RenderingEngine::CreateSwapChain()
{
    m_SwapChain.Reset();

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = CPrioEngine::GetInstance().GetClientWidth();
    sd.BufferDesc.Height = CPrioEngine::GetInstance().GetClientHeight();
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = m_BackBufferFormat;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
    sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = SwapChainBufferCount;
    sd.OutputWindow = CPrioEngine::GetInstance().GetWindowHandle();
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    PrioEngine::ThrowIfFailed(m_dxgiFactory->CreateSwapChain(m_CommandQueue.Get(), &sd, m_SwapChain.GetAddressOf()));
}

void CD3D12RenderingEngine::FlushCommandQueue()
{
    m_CurrentFence++;

    PrioEngine::ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), m_CurrentFence));

    if (m_Fence->GetCompletedValue() < m_CurrentFence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, L"", false, EVENT_ALL_ACCESS);

        PrioEngine::ThrowIfFailed(m_Fence->SetEventOnCompletion(m_CurrentFence, eventHandle));

        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

void CD3D12RenderingEngine::CreateRtvAndDsvDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_RtvHeap.GetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDesc.NodeMask = 0;
    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_DsvHeap.GetAddressOf())));
}

void CD3D12RenderingEngine::OnResize()
{
    if (!m_d3dDevice)
    {
        throw DirectXDeviceNotInitialised();
    }
    else if (!m_SwapChain)
    {
        throw SwapChainNotInitialised();
    }
    else if (!m_DirectCmdListAlloc)
    {
        throw DirectXCommandListAllocationNotInitialised();
    }

    FlushCommandQueue();

    PrioEngine::ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

    // Release the previous resources we will need to recreate
    for (int i = 0; i < SwapChainBufferCount; ++i)
    {
        m_SwapChainBuffer[i].Reset();
    }
    m_DepthStencilBuffer.Reset();

    // Resize the swap chain
    PrioEngine::ThrowIfFailed(m_SwapChain->ResizeBuffers(
        SwapChainBufferCount,
        CPrioEngine::GetInstance().GetClientWidth(), CPrioEngine::GetInstance().GetClientHeight(),
        m_BackBufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    ));

    m_CurrentBackBuffer = 0;

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < SwapChainBufferCount; i++)
    {
        PrioEngine::ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i])));
        m_d3dDevice->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
    }

    // Create the depth/stencil buffer and view.
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = CPrioEngine::GetInstance().GetClientWidth();
    depthStencilDesc.Height = CPrioEngine::GetInstance().GetClientHeight();
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;

    depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

    depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
    depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = m_DepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())));

    // Create descriptor to mip level 0 of entire resource using the format of the resource.
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Format = m_DepthStencilFormat;
    dsvDesc.Texture2D.MipSlice = 0;
    m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

    // Transition the resource from its initial state to be used as a depth buffer.
    m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

    // Execute the resize commands.
    PrioEngine::ThrowIfFailed(m_CommandList->Close());
    ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until resize is complete.
    FlushCommandQueue();

    // Update the viewport transform to cover the client area.
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(CPrioEngine::GetInstance().GetClientWidth());
    m_ScreenViewport.Height = static_cast<float>(CPrioEngine::GetInstance().GetClientHeight());
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_ScissorRect = { 0, 0, CPrioEngine::GetInstance().GetClientWidth(), CPrioEngine::GetInstance().GetClientHeight() };
}

void CD3D12RenderingEngine::Draw()
{
    PrioEngine::ThrowIfFailed(m_DirectCmdListAlloc->Reset());

    PrioEngine::ThrowIfFailed(m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr));

    m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    m_CommandList->RSSetViewports(1, &m_ScreenViewport);
    m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

    m_CommandList->ClearRenderTargetView(CurrentBackBufferView(), DirectX::Colors::LightSteelBlue, 0, nullptr);
    m_CommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    m_CommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

    m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    PrioEngine::ThrowIfFailed(m_CommandList->Close());

    ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
    m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    PrioEngine::ThrowIfFailed(m_SwapChain->Present(0, 0));
    m_CurrentBackBuffer = (m_CurrentBackBuffer + 1) % SwapChainBufferCount;

    FlushCommandQueue();
}

ID3D12Resource* CD3D12RenderingEngine::CurrentBackBuffer() const
{
    return m_SwapChainBuffer[m_CurrentBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CD3D12RenderingEngine::CurrentBackBufferView() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeap->GetCPUDescriptorHandleForHeapStart(), m_CurrentBackBuffer, m_RtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE CD3D12RenderingEngine::DepthStencilView() const
{
    return m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void CD3D12RenderingEngine::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 1;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.NodeMask = 0;
    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_CbvHeap)));
}

void CD3D12RenderingEngine::BuildConstantBuffers()
{
    m_ObjectCB = std::make_unique<CUploadBuffer<PrioEngine::ObjectConstants>>(m_d3dDevice.Get(), 1, true);

    UINT objCBBufferSize = d3dUtil::CalcConstantBufferByteSize(sizeof(PrioEngine::ObjectConstants));
    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = m_ObjectCB->Resource()->GetGPUVirtualAddress();

    int boxCbufIndex = 0;
    cbAddress += boxCbufIndex * objCBBufferSize;

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    cbvDesc.BufferLocation = cbAddress;
    cbvDesc.SizeInBytes = objCBBufferSize;

    m_d3dDevice->CreateConstantBufferView(&cbvDesc, m_CbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void CD3D12RenderingEngine::BuildRootSignature()
{
    // The root sig defines the resources the shader will expect.
    // Resources can be considered any input we will provide to the shader.
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];

    // Create a single descriptor of CBVs
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

    // A root signature is an array of paramaters
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Create a root sig with a single slot which points to the desc range consisting of a single constant buffer
    ComPtr<ID3DBlob> serialisedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serialisedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

    if (errorBlob != nullptr)
    {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    PrioEngine::ThrowIfFailed(hr);

    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateRootSignature(0,
        serialisedRootSig->GetBufferPointer(),
        serialisedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&m_RootSignature))
    );
}

void CD3D12RenderingEngine::BuildShadersAndInputLayout()
{
    HRESULT hr = S_OK;

    using PrioEngine::Rendering::D3D12::CompileShader;

    m_vsByteCode = CompileShader(L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
    m_psByteCode = CompileShader(L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

    m_InputLayout =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    }

}

void CD3D12RenderingEngine::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
    psoDesc.pRootSignature = m_RootSignature.Get();
    psoDesc.VS =
    {
        reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()),
        m_vsByteCode->GetBufferSize()
    };

    psoDesc.PS =
    {
        reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()),
        m_psByteCode->GetBufferSize()
    };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = m_BackBufferFormat;
    psoDesc.SampleDesc.Count = m_4xMsaaQuality ? 4 : 1;
    psoDesc.SampleDesc.Quality = m_4xMsaaQuality ? (m_4xMsaaState - 1) : 0;
    psoDesc.DSVFormat = m_DepthStencilFormat;
    PrioEngine::ThrowIfFailed(m_d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO)));
}

#ifdef _DEBUG
void CD3D12RenderingEngine::LogAdapters()
{
    UINT i = 0;
    IDXGIAdapter* adapter = nullptr;
    std::vector<IDXGIAdapter*> adapterList;

    while (m_dxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text = L"***Adapter: ";
        text += desc.Description;
        text += L"\r\n";

        OutputDebugString(text.c_str());
        adapterList.push_back(adapter);

        i++;
    }

    for (IDXGIAdapter* adapter : adapterList)
    {
        LogAdapterOutputs(adapter);

        // Release the resources from this debug method so we don't keep them locked for others.
        if (adapter)
        {
            adapter->Release();
            adapter = nullptr;
        }
    }
}

void CD3D12RenderingEngine::LogAdapterOutputs(IDXGIAdapter* adapter)
{
    UINT i = 0;
    IDXGIOutput* output = nullptr;
    while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);

        std::wstring text = L"***Output: ";
        text += desc.DeviceName;
        text += L"\n";
        OutputDebugString(text.c_str());

        LogOutputDisplayModes(output, m_BackBufferFormat);

        ReleaseCom(output);

        ++i;
    }
}

void CD3D12RenderingEngine::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
    UINT count = 0;
    UINT flags = 0;

    // Call with nullptr to get list count.
    output->GetDisplayModeList(format, flags, &count, nullptr);

    std::vector<DXGI_MODE_DESC> modeList(count);
    output->GetDisplayModeList(format, flags, &count, &modeList[0]);

    for (auto& x : modeList)
    {
        UINT n = x.RefreshRate.Numerator;
        UINT d = x.RefreshRate.Denominator;
        std::wstring text =
            L"Width = " + std::to_wstring(x.Width) + L" " +
            L"Height = " + std::to_wstring(x.Height) + L" " +
            L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
            L"\n";

        ::OutputDebugString(text.c_str());
    }
}
#endif
