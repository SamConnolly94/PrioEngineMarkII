#pragma once
#include <Windows.h>
#include <vector>
#include "d3dUtil.h"
#include "GameTimer.h"
#include "EEngineCodes.h"
#include "ShaderFacade.h"
#include "ObjectConstants.h"
#include "UploadBuffer.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

namespace PrioEngineII
{
	class PrioEngine : std::enable_shared_from_this< PrioEngine >
	{
	private:
		static std::shared_ptr<PrioEngine> EngineInstance;
	public:
		static std::shared_ptr<PrioEngine> GetInstance();
	protected:
		PrioEngine(HINSTANCE windowHandle);
	public:
		virtual EEngineCodes Initialise(std::shared_ptr<PrioEngine> engineInstance);
		EEngineCodes DrawFrame();
		bool IsRunning();
		HINSTANCE GetAppInstance() const;
		bool Get4xMsaaState() const;
		void Set4xMsaaState(bool value);
		virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		float AspectRatio() const;
		HWND MainWnd() const;
		float GetDeltaTime() const;
	protected:
		virtual void Draw() = 0;
		bool InitMainWindow();

		void CalculateFrameStats();
		virtual void CreateDxgiFactory() = 0;
		virtual void CreateCommandObjects() = 0;
		virtual void FlushCommandQueue() = 0;
		virtual void OnResize() = 0;
		virtual void CreateDescriptorHeaps() = 0;
		virtual void InitialiseDebugLayer() = 0;
		virtual void InitialiseFence() = 0;
		virtual void InitialiseMultisampling() = 0;
		virtual void InitialiseDescriptorSizes() = 0;
		virtual bool CreateDirectXDevice() = 0;
		virtual void LogAdapters() = 0;
		virtual void InitSwapChain(DXGI_SWAP_CHAIN_DESC& sd) = 0;
		virtual void BuildDefaultShaders();

		// Convenience overrides for handling mouse input.
		virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
		virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
		virtual void OnMouseMove(WPARAM btnState, int x, int y) { }
		virtual bool GraphicsApiInitialised() const = 0;
		virtual void BuildConstantBuffers() = 0;
	private:
		bool InitGraphicsAPI();
		void CreateSwapChain();
	protected:
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		HINSTANCE hInstance;
		HWND      mhMainWnd = nullptr; // main window handle
		bool      mAppPaused = false;  // is the application paused?
		bool      mMinimized = false;  // is the application minimized?
		bool      mMaximized = false;  // is the application maximized?
		bool      mResizing = false;   // are the resize bars being dragged?
		bool      mFullscreenState = false;// fullscreen enabled
		DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		static const int SwapChainBufferCount = 2;
		std::vector<ShaderFacade> mShaders;

		// Set true to use 4X MSAA (§4.1.8).  The default is false.
		bool      m4xMsaaState = false;    // 4X MSAA enabled
		UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

		// Used to keep track of the “delta-time” and game time (§4.4).
		std::unique_ptr<GameTimer> mTimer;

		std::wstring mMainWndCaption = L"Prio Engine Mark II";

		int mClientWidth = 800;
		int mClientHeight = 600;

		std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectConstantBuffer = nullptr;

		bool Quit = false;
	};
}