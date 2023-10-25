#pragma once

#include <string>

enum class EGraphicsAPI;

class CRenderingEngineBase
{
public:
    explicit CRenderingEngineBase(const EGraphicsAPI& graphicsApi);
    CRenderingEngineBase() = delete;

    EGraphicsAPI GetGraphicsAPI() const;
    virtual void OnResize() = 0;
    virtual void Draw() = 0;
    std::wstring CalculateFrameStats(const float& totalTime);

    bool Get4xMsaaState() const { return m_4xMsaaState; };
    void Set4xMsaaState(const bool value) { m_4xMsaaState = value; };
protected:
    bool m_4xMsaaState{ false };
public:
    virtual bool Initialise() = 0;
protected:
    EGraphicsAPI m_GraphicsApi;
private:
    int m_FrameCount{ 0 };
    float m_TimeElapsed{ 0.0f };
};

