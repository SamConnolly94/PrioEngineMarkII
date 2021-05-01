#pragma once

#include "PrioEngine.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <memory>
#include <d3d12.h>

// Custom includes
#include "d3dRenderTargetViewHeap.h"
#include "d3dDepthStencilViewHeap.h"
#include "d3dConstantBufferViewHeap.h"
#include "d3dDepthStencilView.h"
#include "Box.h"

// Link necessary d3d12 libraries.
#pragma comment(lib, "D3D12.lib")

using namespace DirectX;

namespace PrioEngineII
{

	class D3D12PrioEngine : public PrioEngine
	{
		friend class PrioEngineFactory;
		struct _constructor_tag { explicit _constructor_tag() = default; };
	public:
		/// <summary>
		/// Private constructor to avoid being created outside of factory
		/// </summary>
		/// <param name="unique_ptr_tag">The tag only accessible within the class to enable unique pointer creation</param>
		D3D12PrioEngine(_constructor_tag unique_ptr_tag, HINSTANCE hInstance);
	private:
		/// <summary>
		/// Factory method for creating an instance of this engine
		/// </summary>
		/// <returns>A unique pointer to the created engine</returns>
		static std::shared_ptr<D3D12PrioEngine> Factory(HINSTANCE hInstance)
		{
			return std::move(std::make_shared<D3D12PrioEngine>(_constructor_tag{}, hInstance));
		}

	protected:
		virtual void OnResize() override;
		virtual void Draw() override;
		virtual void Update() override;
	protected:
		virtual void CreateDxgiFactory() override;
		virtual void InitialiseDebugLayer() override;
		virtual void InitialiseFence() override;
		virtual void InitialiseMultisampling() override;
		virtual void InitialiseDescriptorSizes() override;
		virtual bool CreateDirectXDevice() override;
		virtual void CreateCommandObjects() override;
		virtual void FlushCommandQueue() override;
		virtual void LogAdapters() override;
		virtual void CreateDescriptorHeaps() override;
		virtual void InitSwapChain(DXGI_SWAP_CHAIN_DESC& sd) override;
		virtual void BuildRootSignature() override;
		virtual void LoadDefaultShaders() override;
		virtual void SetShaderInputLayout() override;
		virtual void LoadDefaultGeometry() override;
		virtual void BuildPSO() override;
		virtual void ResetCommandList() override;
		virtual void ExecuteCommandList() override;

		ID3D12Resource* CurrentBackBuffer() const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

		void LogAdapterOutputs(IDXGIAdapter* adapter);
		void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

		virtual bool GraphicsApiInitialised() const override;
		virtual void BuildConstantBuffers() override;
	protected:

		ComPtr<IDXGIFactory4> mdxgiFactory;
		ComPtr<ID3D12Device> md3dDevice;
		ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

		ComPtr<ID3D12Fence> mFence;
		UINT64 mCurrentFence = 0;

		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;

		ComPtr<ID3D12PipelineState> mPSO = nullptr;

		int mCurrBackBuffer = 0;
		Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
		std::unique_ptr<d3dDepthStencilView<ID3D12Device, ID3D12Resource, D3D12_CPU_DESCRIPTOR_HANDLE>> mDepthStencilBuffer;

		std::unique_ptr<d3dRenderTargetViewHeap<ID3D12Device, ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC>> mRenderTargetViewHeap;
		std::unique_ptr<d3dDepthStencilViewHeap<ID3D12Device, ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC>> mDepthStencilViewHeap;
		std::unique_ptr<d3dConstantBufferViewHeap<ID3D12Device, ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC>> mConstantBufferViewHeap;

		D3D12_VIEWPORT mScreenViewport;
		D3D12_RECT mScissorRect;

		UINT mRtvDescriptorSize = 0;
		UINT mDsvDescriptorSize = 0;
		UINT mCbvSrvUavDescriptorSize = 0;

		// Derived class should set these in derived constructor to customize starting values.
		D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;

		std::unique_ptr<Box<ID3D12Device, ID3D12GraphicsCommandList>> mBox;
	};
}

