#include <pch.h>
#include <engine/rendering/shapes/box.h>

using namespace PrioEngine;
using namespace DirectX;
using namespace PrioEngine::Math;

namespace PrioEngine
{
    UINT CBox::GetVertexBufferByteSize() const
    {
        return (UINT)m_Vertices.size() * sizeof(Vertex);
    }

    UINT CBox::GetIndexBuferByteSize() const
    {
        return (UINT)m_Indices.size() * sizeof(std::uint16_t);
    }

    std::array<Math::Vertex, 8> CBox::GetVertices() const
    {
        return m_Vertices;
    }

    std::array<std::uint16_t, 36> CBox::GetIndices() const
    {
        return m_Indices;
    }
}