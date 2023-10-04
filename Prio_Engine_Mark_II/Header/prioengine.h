#pragma once

#include <types/graphicsapi.h>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class CPrioEngine
{
public:
	CPrioEngine(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, 
		unsigned int width = 800, 
		unsigned int height = 600, 
		const char* windowTitle = "Prio Engine II");
	CPrioEngine() = delete;
protected:
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();
private:
	static const unsigned int kFrameCount = 2;
	
	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_ScissorRect;	// TODO: What is this for? Is this really generic?
	ComPtr<IDXGISwapChain3> m_SwapChain;
	ComPtr<ID3D12Device> m_Device;
	ComPtr<ID3D12Resource> m_RenderTargets[kFrameCount];
	ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
	ComPtr<ID3D12CommandQueue> m_CommandQueue;
	ComPtr<ID3D12RootSignature> m_CootSignature;
	ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
	ComPtr<ID3D12PipelineState> m_PipelineState;
	ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	UINT m_RtvDescriptorSize;

	// App resources.
	ComPtr<ID3D12Resource> m_VertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;

	// Synchronization objects.
	UINT m_FrameIndex;
	HANDLE m_FenceEvent;
	ComPtr<ID3D12Fence> m_Fence;
	UINT64 m_FenceValue;

	bool m_UseWarpDevice{false};

	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();
	void WaitForPreviousFrame();
};

