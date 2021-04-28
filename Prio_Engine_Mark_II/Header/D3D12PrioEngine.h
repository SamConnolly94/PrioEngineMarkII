#pragma once

#include "PrioEngine.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <memory>

// Custom includes
#include "d3dRenderTargetView.h"
#include "d3dDepthStencilView.h"

// Link necessary d3d12 libraries.
#pragma comment(lib, "D3D12.lib")


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
		virtual void CreateRtvAndDsvDescriptorHeaps();
		virtual void OnResize() override;
		virtual void Draw() override;
	protected:
		virtual bool InitGraphicsAPI() override;
		virtual void CreateCommandObjects() override;
		virtual void CreateSwapChain() override;
		virtual void FlushCommandQueue() override;

		ID3D12Resource* CurrentBackBuffer() const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

		void LogAdapters();
		void LogAdapterOutputs(IDXGIAdapter* adapter);
		void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

		virtual bool GraphicsApiInitialised() const override;
	protected:

		Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
		UINT64 mCurrentFence = 0;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

		static const int SwapChainBufferCount = 2;
		int mCurrBackBuffer = 0;
		Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
		Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

		std::unique_ptr<PrioEngineII::d3dRenderTargetView<ID3D12Device, ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC>> mRenderTargetView;
		std::unique_ptr<PrioEngineII::d3dDepthStencilView<ID3D12Device, ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC>> mDepthStencilView;

		D3D12_VIEWPORT mScreenViewport;
		D3D12_RECT mScissorRect;

		UINT mRtvDescriptorSize = 0;
		UINT mDsvDescriptorSize = 0;
		UINT mCbvSrvUavDescriptorSize = 0;

		// Derived class should set these in derived constructor to customize starting values.
		D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	};
}
