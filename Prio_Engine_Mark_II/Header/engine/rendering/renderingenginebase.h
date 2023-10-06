#pragma once

#include <types/graphicsapi.h>

class CRenderingEngineBase
{
public:
    CRenderingEngineBase(const EGraphicsAPI& graphicsApi);

    EGraphicsAPI GetGraphicsAPI() const;
protected:
    bool InitWindow();
    virtual bool InitGraphicsApi() = 0;
    void OnResize();
private:
    bool Initialize();
protected:
    EGraphicsAPI m_GraphicsApi{EGraphicsAPI::DX12};
};

