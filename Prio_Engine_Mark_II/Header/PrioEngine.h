#pragma once
#include <Windows.h>
#include "d3dUtil.h"
#include "GameTimer.h"
#include "EEngineCodes.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
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

		virtual bool InitGraphicsAPI() = 0;
		void CalculateFrameStats();
		virtual void CreateCommandObjects() = 0;
		virtual void CreateSwapChain() = 0;
		virtual void FlushCommandQueue() = 0;
		virtual void OnResize() = 0;

		// Convenience overrides for handling mouse input.
		virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
		virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
		virtual void OnMouseMove(WPARAM btnState, int x, int y) { }
		virtual bool GraphicsApiInitialised() const = 0;
	protected:
		HINSTANCE hInstance;
		HWND      mhMainWnd = nullptr; // main window handle
		bool      mAppPaused = false;  // is the application paused?
		bool      mMinimized = false;  // is the application minimized?
		bool      mMaximized = false;  // is the application maximized?
		bool      mResizing = false;   // are the resize bars being dragged?
		bool      mFullscreenState = false;// fullscreen enabled

		// Set true to use 4X MSAA (§4.1.8).  The default is false.
		bool      m4xMsaaState = false;    // 4X MSAA enabled
		UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

		// Used to keep track of the “delta-time” and game time (§4.4).
		std::unique_ptr<GameTimer> mTimer;

		std::wstring mMainWndCaption = L"Prio Engine Mark II";

		int mClientWidth = 800;
		int mClientHeight = 600;

		bool Quit = false;
	};
}