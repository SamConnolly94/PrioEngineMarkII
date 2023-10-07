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
class CTimer;

class CPrioEngine
{
public:
    CPrioEngine() = delete;
    CPrioEngine(CPrioEngine&) = delete;
    void operator=(const CPrioEngine&) = delete;

    static bool __declspec(dllexport) HasInstance() { return m_Instance != nullptr; };
    static void __declspec(dllexport) CreateInstance(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, unsigned int width = 800, unsigned int height = 600, const std::string windowTitle = "Prio Engine II");
    static __declspec(dllexport) CPrioEngine& GetInstance();

    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    bool Initialise();

    int __declspec(dllexport) GetClientWidth() const { return m_ClientWidth; };
    int __declspec(dllexport) GetClientHeight() const { return m_ClientHeight; };
    HWND __declspec(dllexport) GetWindowHandle() const { return mh_MainWnd; };
    bool __declspec(dllexport) Update();

    // Convenience overrides for handling mouse input.
    virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
    virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
    virtual void OnMouseMove(WPARAM btnState, int x, int y) { }

protected:
    CPrioEngine(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, unsigned int width = 800, unsigned int height = 600, const std::string windowTitle = "Prio Engine II");
    void OnResize();
private:
    bool InitMainWindow();
protected:
    bool m_Paused{ false };
private:
    inline static CPrioEngine* m_Instance;
    std::unique_ptr<CRenderingEngineBase> m_RenderingEngine{};
    HWND mh_MainWnd{};
    HINSTANCE mh_Instance{};
    int m_ClientWidth{ 800 };
    int m_ClientHeight{ 600 };
    bool m_Minimised{ false };
    bool m_Maximised{ false };
    bool m_Resizing{ false };
    std::wstring m_WindowTitle{};
    std::unique_ptr<CTimer> m_Timer;
};

