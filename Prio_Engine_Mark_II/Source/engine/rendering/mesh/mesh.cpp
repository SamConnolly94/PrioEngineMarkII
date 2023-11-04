#include <pch.h>
#include <engine/rendering/mesh/mesh.h>

D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VertexBufferView() const
{
    D3D12_VERTEX_BUFFER_VIEW vbv;
    vbv.BufferLocation = m_VertexBufferGPU->GetGPUVirtualAddress();
    vbv.StrideInBytes = m_VertexByteStride;
    vbv.SizeInBytes = m_VertexBufferByteSize;
    return vbv;
}

D3D12_INDEX_BUFFER_VIEW MeshGeometry::IndexBufferView() const
{
    D3D12_INDEX_BUFFER_VIEW ibv;
    ibv.BufferLocation = m_IndexBufferCPU->GetGPUVirtualAddress();
    ibv.Format = m_IndexFormat;
    ibv.SizeInBytes = m_IndexBufferByteSize;
    return ibv;
}

void MeshGeometry::DisposeUploaders()
{
    m_VertexBufferUploader.Reset();
    m_IndexBufferUploader.Reset();
}