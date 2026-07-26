#pragma once

#include <string>
#include <DirectXMath.h>
#include <memory>

enum class EGraphicsAPI;
class CInputHandler;

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
    std::shared_ptr<CInputHandler> m_InputHandler;
public:
    virtual bool Initialise(std::shared_ptr<CInputHandler> inputHandler) = 0;
    virtual bool GraphicsApiInitialised() = 0;
protected:
    EGraphicsAPI m_GraphicsApi;
private:
    int m_FrameCount{ 0 };
    float m_TimeElapsed{ 0.0f };
};

