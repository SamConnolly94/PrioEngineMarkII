#pragma once

#include <engine/rendering/renderingenginebase.h>

class CD3D12RenderingEngine : public CRenderingEngineBase
{
    bool InitGraphicsApi() override;
};

