#pragma once

#include <types/graphicsapi.h>
#include <memory>

class CRenderingEngineBase;

class CPrioEngine
{
public:
    CPrioEngine() = delete;
    CPrioEngine(EGraphicsAPI graphicsApi = EGraphicsAPI::DX12, unsigned int width = 800, unsigned int height = 600, const char* windowTitle = "Prio Engine II");
protected:
    
private:
    std::unique_ptr<CRenderingEngineBase> m_RenderingEngine;
};

