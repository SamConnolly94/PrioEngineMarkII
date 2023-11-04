#pragma once

#include <array>

namespace PrioEngine
{
    namespace Math
    {
        struct Vertex;
    }

    class CBox
    {
    public:
        CBox();
        UINT GetVertexBufferByteSize() const;
        UINT GetIndexBuferByteSize() const;

        std::array<Math::Vertex, 8> GetVertices() const;
        std::array<std::uint16_t, 36> GetIndices() const;
    private:
        std::array<Math::Vertex, 8> m_Vertices;
        std::array<std::uint16_t, 36> m_Indices;
    };
}