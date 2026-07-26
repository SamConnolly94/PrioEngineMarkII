#include "pch.h"

#include <engine/input/inputhandler.h>

#include <algorithm>
#include <WinUser.h>

CInputHandler::CInputHandler(HWND hWnd)
{
    mh_MainWnd = hWnd;
}

void CInputHandler::OnMouseDown(WPARAM btnState, int x, int y)
{
    m_LastMousePosition.x = x;
    m_LastMousePosition.y = y;

    SetCapture(mh_MainWnd);
}

void CInputHandler::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void CInputHandler::OnMouseMove(WPARAM btnState, int x, int y)
{
    if ((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePosition.x));
        float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePosition.y));

        // Update angles based on input to orbit camera around box.
        m_Theta += dx;
        m_Phi += dy;

        // Restrict the angle mPhi.
        m_Phi = std::clamp(m_Phi, 0.1f, DirectX::XM_PI - 0.1f);
    }
    else if ((btnState & MK_RBUTTON) != 0)
    {
        // Make each pixel correspond to 0.005 unit in the scene.
        float dx = 0.005f * static_cast<float>(x - m_LastMousePosition.x);
        float dy = 0.005f * static_cast<float>(y - m_LastMousePosition.y);

        // Update the camera radius based on input.
        m_Radius += dx - dy;

        // Restrict the radius.
        m_Radius = std::clamp(m_Radius, 3.0f, 15.0f);
    }

    m_LastMousePosition.x = x;
    m_LastMousePosition.y = y;
}

float CInputHandler::GetTheta()
{
    return m_Theta;
}

float CInputHandler::GetPhi()
{
    return m_Phi;
}

float CInputHandler::GetRadius()
{
    return m_Radius;
}
