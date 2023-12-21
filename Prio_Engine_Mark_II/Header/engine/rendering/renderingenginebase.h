#pragma once

#include <string>
#include <DirectXMath.h>

enum class EGraphicsAPI;

class CRenderingEngineBase
{
public:
    explicit CRenderingEngineBase(const EGraphicsAPI& graphicsApi);
    CRenderingEngineBase() = delete;

    EGraphicsAPI GetGraphicsAPI() const;
    virtual void OnResize() = 0;
    virtual void Update();
    virtual void Draw() = 0;
    std::wstring CalculateFrameStats(const float& totalTime);

    bool Get4xMsaaState() const { return m_4xMsaaState; };
    void Set4xMsaaState(const bool value) { m_4xMsaaState = value; };
protected:
    virtual void UpdateCameraMatrices() = 0;
protected:
    bool m_4xMsaaState{ false };
    // TODO:
    // These are probably gameplay specific things that should be passed in.
    // THey are here for the box rendering tutorial.
    // I could remove them afterwards
    static constexpr float m_Radius{ 5.0f };
    static constexpr float m_Phi{ DirectX::XM_PIDIV4 };
    static constexpr float m_Theta = { 1.5f * DirectX::XM_PI };
    // end TODO
public:
    virtual bool Initialise() = 0;
protected:
    EGraphicsAPI m_GraphicsApi;
private:
    int m_FrameCount{ 0 };
    float m_TimeElapsed{ 0.0f };
};

