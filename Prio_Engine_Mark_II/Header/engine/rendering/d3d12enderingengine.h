#pragma once

#include <engine/rendering/renderingenginebase.h>
#include <d3d12.h>
#include <wrl.h>

class CD3D12RenderingEngine : public CRenderingEngineBase
{
public:
    bool InitGraphicsApi() override;
    Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() const;
    
private:
    Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;
};

