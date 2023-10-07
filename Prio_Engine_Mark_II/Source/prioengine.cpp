#include <pch.h>
#include <prioengine.h>

#include <engine/timer.h>
#include <engine/rendering/d3d12enderingengine.h>
#include <engine/rendering/exceptions/renderingengineexception.h>
#include <engine/common/stringconversion.h>

#include <os/globals.h>
#include <windowsx.h>

using Microsoft::WRL::ComPtr;
using namespace std;

HINSTANCE PrioEngine::Globals::OS::gh_InstDll{ NULL };

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:

        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.
        break;
    }

    PrioEngine::Globals::OS::gh_InstDll = hinstDLL;
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return CPrioEngine::GetInstance().MsgProc(hwnd, msg, wParam, lParam);
}

void CPrioEngine::CreateInstance(EGraphicsAPI graphicsApi, unsigned int width, unsigned int height, const std::string windowTitle)
{
    if (!m_Instance)
    {
        // TODO:
        // This is currently leaking.
        // Should really clean up, or change to be a solid object.
        m_Instance = new CPrioEngine(graphicsApi, width, height, windowTitle);
        m_Instance->Initialise();
    }
}

CPrioEngine& CPrioEngine::GetInstance()
{
    return *m_Instance;
}

CPrioEngine::CPrioEngine(EGraphicsAPI graphicsApi, unsigned int width, unsigned int height, const std::string windowTitle)
{
    m_WindowTitle = PrioEngine::Common::StringToWideString(windowTitle);
    m_ClientWidth = width;
    m_ClientHeight = height;
    mh_Instance = PrioEngine::Globals::OS::gh_InstDll;
    m_Timer = std::make_unique<CTimer>();

    switch (graphicsApi)
    {
    case EGraphicsAPI::DX12:
    {
        m_RenderingEngine = std::make_unique<CD3D12RenderingEngine>(EGraphicsAPI::DX12);
        break;
    }
    default:
        throw RenderingEngineNotSupported();
        return;
    }
}

bool CPrioEngine::Initialise()
{
    if (!InitMainWindow())
    {
        return false;
    }

    if (!m_RenderingEngine->Initialise())
    {
        return false;
    }

    OnResize();

    return false;
}

bool CPrioEngine::InitMainWindow()
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = mh_Instance;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = L"MainWnd";

    if (!RegisterClass(&wc))
    {
        MessageBox(0, L"RegisterClass Failed.", 0, 0);
        return false;
    }

    RECT R = { 0, 0, static_cast<int>(m_ClientWidth), static_cast<int>(m_ClientHeight) };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
    int width = R.right - R.left;
    int height = R.bottom - R.top;

    mh_MainWnd = CreateWindow(L"MainWnd", m_WindowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mh_Instance, 0);

    if (!mh_MainWnd)
    {
        MessageBox(0, L"CreateWindow Failed.", 0, 0);
        return false;
    }

    ShowWindow(mh_MainWnd, SW_SHOW);
    UpdateWindow(mh_MainWnd);

    return true;
}

void CPrioEngine::OnResize()
{
    m_RenderingEngine->OnResize();
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
            m_Timer->Stop();
        }
        else
        {
            m_Paused = false;
            m_Timer->Start();
            //m_Paused = false;
            //m_Timer->Start();
        }
        return 0;

        // WM_SIZE is sent when the user resizes the window.  
    case WM_SIZE:
    {
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
                if (m_Minimised)
                {
                    m_Paused = false;
                    m_Minimised = false;
                    OnResize();
                }

                // Restoring from maximized state?
                else if (m_Maximised)
                {
                    m_Paused = false;
                    m_Maximised = false;
                    OnResize();
                }
                else if (m_Resizing)
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
    }
    // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
    case WM_ENTERSIZEMOVE:
        m_Paused = true;
        m_Resizing = true;
        m_Timer->Stop();
        return 0;

        // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
        // Here we reset everything based on the new window dimensions.
    case WM_EXITSIZEMOVE:
        m_Paused = false;
        m_Resizing = false;
        m_Timer->Start();
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
        {
            // Toggle anti aliasing state
            m_RenderingEngine->Set4xMsaaState(!m_RenderingEngine->Get4xMsaaState());
        }

        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CPrioEngine::Update()
{
    MSG msg{ 0 };
    m_Timer->Reset();

    if (msg.message == WM_QUIT)
    {
        return false;
    }

    // Process windows messages
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // Update game specific stuff
    else
    {
        m_Timer->Tick();

        if (m_Paused)
        {
            std::wstring frameStats = m_RenderingEngine->CalculateFrameStats(m_Timer->TotalTime());

            if (frameStats != L"")
            {
                SetWindowText(mh_MainWnd, frameStats.c_str());
            }

            m_RenderingEngine->Draw();
        }
        else
        {
            Sleep(100);
        }
    }

    return true;
}