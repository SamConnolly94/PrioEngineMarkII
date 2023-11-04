#pragma once

#include <engine/rendering/helper/d3d12util.h>

using Microsoft::WRL::ComPtr;
using namespace std;

struct MeshGeometry
{
    std::string m_Name;

    Microsoft::WRL::ComPtr<ID3DBlob> m_VertexBufferCPU{ nullptr };
    Microsoft::WRL::ComPtr<ID3DBlob> m_IndexBufferCPU{ nullptr };

    Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBufferGPU{ nullptr };
    Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBufferGPU{ nullptr };

    Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBufferUploader{ nullptr };
    Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBufferUploader{ nullptr };

    UINT m_VertexByteStride{ 0 };
    UINT m_VertexBufferByteSize{ 0 };
    DXGI_FORMAT m_IndexFormat{ DXGI_FORMAT_R16_UINT };
    UINT m_IndexBufferByteSize{ 0 };

    std::unordered_map<std::string, SubmeshGeometry> m_DrawArgs;

    D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;
    D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;

    void DisposeUploaders();
};