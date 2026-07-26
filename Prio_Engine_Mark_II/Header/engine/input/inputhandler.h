#pragma once

#include <minwindef.h>
#include <DirectXMath.h>
#include <windef.h>

class CInputHandler
{
public:
    CInputHandler(HWND hWnd);
    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

    float GetTheta();
    float GetPhi();
    float GetRadius();
private:
    HWND mh_MainWnd{};

    float m_Theta = 1.5f * DirectX::XM_PI;
    float m_Phi = DirectX::XM_PIDIV4;
    float m_Radius = 5.0f;

    POINT m_LastMousePosition{ 0, 0 };
};
