#include <pch.h>
#include <prioengine.h>

#include <engine/rendering/d3d12enderingengine.h>
#include <engine/rendering/exceptions/renderingengineexception.h>

CPrioEngine::CPrioEngine(EGraphicsAPI graphicsApi, unsigned int width, unsigned int height, const char* windowTitle)
{
    switch (graphicsApi)
    {
    case EGraphicsAPI::DX12:
    {
        m_RenderingEngine = std::make_unique<CD3D12RenderingEngine>();
        break;
    }
    default:
        throw RenderingEngineNotSupported();
    }
}