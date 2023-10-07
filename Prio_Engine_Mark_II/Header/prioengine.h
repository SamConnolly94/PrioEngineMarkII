#pragma once

#include <engine/rendering/types/graphicsapi.h>
#include <memory>
#include <Windows.h>
#include <wrl.h>
#include <string>

// TODO:
// Don't believe this is needed.
using Microsoft::WRL::ComPtr;
using namespace std;

class CRenderingEngineBase;

class CPrioEngine
{
public:
    CPrioEngine() = delete;
    CPrioEngine(CPrioEngine&) = delete;
    void operator=(const CPrioEngine&) = delete;

    static void CreateInstance(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, unsigned int width = 800, unsigned int height = 600, const std::string windowTitle = "Prio Engine II");
    static CPrioEngine& GetInstance();

    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool Initialise();

    int GetClientWidth() const { return m_ClientWidth; };
    int GetClientHeight() const { return m_ClientHeight; };
    HWND GetWindowHandle() const { return mh_MainWnd; };
    bool Update();
protected:
    void OnResize();
private:
    CPrioEngine(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, unsigned int width = 800, unsigned int height = 600, const std::string windowTitle = "Prio Engine II");
    bool InitMainWindow();
protected:
    bool m_Paused{ false };
private:
    static std::unique_ptr<CPrioEngine> m_Instance;
    std::unique_ptr<CRenderingEngineBase> m_RenderingEngine{};
    HWND mh_MainWnd{};
    HINSTANCE mh_Instance{};
    int m_ClientWidth{ 800 };
    int m_ClientHeight{ 600 };
    bool m_Minimised{ false };
    bool m_Maximised{ false };
    std::wstring m_WindowTitle{};
};

