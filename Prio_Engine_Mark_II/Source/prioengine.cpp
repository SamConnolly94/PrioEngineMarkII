#include <pch.h>
#include <prioengine.h>

#include <engine/rendering/d3d12enderingengine.h>
#include <engine/rendering/exceptions/renderingengineexception.h>

CPrioEngine::CPrioEngine(EGraphicsAPI graphicsApi, unsigned int width, unsigned int height, const char* windowTitle)
{
    switch (graphicsApi)
    {
    case EGraphicsAPI::DX12:
    {
        m_RenderingEngine = std::make_unique<CD3D12RenderingEngine>();
        break;
    }
    default:
        throw RenderingEngineNotSupported();
    }
}

void CPrioEngine::CreateInstance(EGraphicsAPI graphicsApi, unsigned int width, unsigned int height, const char* windowTitle)
{
	if (!m_Instance)
	{
		m_Instance = std::make_unique<CPrioEngine>(CPrioEngine(graphicsApi, width, height, windowTitle));
	}
}

CPrioEngine& CPrioEngine::GetInstance()
{
	return *m_Instance.get();
}

LRESULT CPrioEngine::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_Paused = true;
			mTimer.Stop();
		}
		else
		{
			m_Paused = false;
			mTimer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_ClientWidth = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);

		CD3D12RenderingEngine* dxRenderingEnginePtr = dynamic_cast<CD3D12RenderingEngine*>(m_RenderingEngine.get());
		if (dxRenderingEnginePtr && dxRenderingEnginePtr->GetDevice())
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_Paused = true;
				m_Minimised = true;
				m_Maximised = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_Paused = false;
				m_Minimised = false;
				m_Maximised = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		mTimer.Start();
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if ((int)wParam == VK_F2)
			Set4xMsaaState(!m4xMsaaState);

		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
