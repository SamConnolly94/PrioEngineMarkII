#pragma once

#include <string>

enum class EGraphicsAPI;

class CRenderingEngineBase
{
public:
    CRenderingEngineBase(const EGraphicsAPI& graphicsApi);

    EGraphicsAPI GetGraphicsAPI() const;
    virtual void OnResize() = 0;
    virtual void Draw() = 0;
    std::wstring CalculateFrameStats(const float& totalTime);
protected:
    virtual bool InitGraphicsApi() = 0;
public:
    virtual bool Initialise() = 0;
protected:
    EGraphicsAPI m_GraphicsApi;
};

