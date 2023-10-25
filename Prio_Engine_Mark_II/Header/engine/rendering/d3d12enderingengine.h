#pragma once

#include <engine/rendering/helper/d3d12util.h>
#include <engine/rendering/renderingenginebase.h>
#include <engine/common/objectconstants.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

struct MeshGeometry;

template <typename T>
class CUploadBuffer;

class CD3D12RenderingEngine : public CRenderingEngineBase
{
public:
    using CRenderingEngineBase::CRenderingEngineBase;

    bool Initialise() override;
    Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() const;
protected:
    void CreateCommandObjects();
    void CreateSwapChain();
    void FlushCommandQueue();
    virtual void CreateRtvAndDsvDescriptorHeaps();
    void OnResize() override;
    void Draw() override;

    ID3D12Resource* CurrentBackBuffer() const;
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildBoxGeometry();
    void BuildPSO();

#ifdef _DEBUG
    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);
#endif

private:
    Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
    Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
    UINT64 m_CurrentFence{ 0 };

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_DirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;

    static const int SwapChainBufferCount{ 2 };
    int m_CurrentBackBuffer{ 0 };
    Microsoft::WRL::ComPtr<ID3D12Resource> m_SwapChainBuffer[SwapChainBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DsvHeap;

    D3D12_VIEWPORT m_ScreenViewport;
    D3D12_RECT m_ScissorRect;

    UINT m_RtvDescriptorSize{ 0 };
    UINT m_DsvDescriptorSize{ 0 };
    UINT m_CbvSrvUavDescriptorSize{ 0 };

    D3D_DRIVER_TYPE m_d3dDriverType{ D3D_DRIVER_TYPE_HARDWARE };
    DXGI_FORMAT m_BackBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
    DXGI_FORMAT m_DepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

    UINT m_4xMsaaQuality{ 0 };

    // Further required vars for rendering shapes
    // I'd like to split this out somewhere outside of the rendering class at some point
    // Some of it still belongs in here, but other parts definitely do not.
    ComPtr<ID3D12RootSignature> m_RootSignature{ nullptr };
    ComPtr<ID3D12DescriptorHeap> m_CbvHeap{ nullptr };

    std::unique_ptr<CUploadBuffer<PrioEngine::ObjectConstants>> m_ObjectCB = nullptr;

    std::unique_ptr<MeshGeometry> m_BoxGeometry{ nullptr };

    ComPtr<ID3DBlob> m_vsByteCode{ nullptr };
    ComPtr<ID3DBlob> m_psByteCode{ nullptr };

};

