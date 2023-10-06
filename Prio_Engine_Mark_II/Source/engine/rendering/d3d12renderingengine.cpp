#include <pch.h>
#include <engine/rendering/d3d12enderingengine.h>


Microsoft::WRL::ComPtr<ID3D12Device> CD3D12RenderingEngine::GetDevice() const
{
    return m_d3dDevice;
}
