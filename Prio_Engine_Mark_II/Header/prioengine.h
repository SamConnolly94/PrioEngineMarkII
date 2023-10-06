#pragma once

#include <types/graphicsapi.h>
#include <memory>
#include <Windows.h>
#include <wrl.h>

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

    static void CreateInstance(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, unsigned int width = 800, unsigned int height = 600, const char* windowTitle = "Prio Engine II");
    static CPrioEngine& GetInstance();

    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    CPrioEngine(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, unsigned int width = 800, unsigned int height = 600, const char* windowTitle = "Prio Engine II");
protected:
    bool m_Paused{ false };
private:
    static std::unique_ptr<CPrioEngine> m_Instance;
    std::unique_ptr<CRenderingEngineBase> m_RenderingEngine;
    HWND m_Hwnd;
    HINSTANCE m_Hinstance;
    unsigned int m_ClientWidth{ 800 };
    unsigned int m_ClientHeight{ 600 };
    bool m_Minimised{ false };
    bool m_Maximised{ false };
};

