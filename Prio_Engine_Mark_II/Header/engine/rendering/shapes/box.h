#pragma once

#include <array>
#include <maths/types/vertex.h>

namespace PrioEngine
{
    class CBox
    {
    public:
        UINT GetVertexBufferByteSize() const;
        UINT GetIndexBuferByteSize() const;

        std::array<PrioEngine::Math::Vertex, 8> GetVertices() const;
        std::array<std::uint16_t, 36> GetIndices() const;
    private:
        std::array<PrioEngine::Math::Vertex, 8> m_Vertices = {
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3{-1.0f, -1.0f, -1.0f}, Colour{DirectX::Colors::White}),
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3{-1.0f, +1.0f, -1.0f}, Colour{DirectX::Colors::Black }),
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3(+1.0f, +1.0f, -1.0f), Colour(DirectX::Colors::Red)),
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3(+1.0f, -1.0f, -1.0f), Colour(DirectX::Colors::Green)),
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3(-1.0f, -1.0f, +1.0f), Colour(DirectX::Colors::Blue)),
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3(-1.0f, +1.0f, +1.0f), Colour(DirectX::Colors::Yellow)),
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3(+1.0f, +1.0f, +1.0f), Colour(DirectX::Colors::Cyan)),
            PrioEngine::Math::Vertex(PrioEngine::Math::Vector3(+1.0f, -1.0f, +1.0f), Colour(DirectX::Colors::Magenta))
        };

        std::array<std::uint16_t, 36> m_Indices = {
            // front face
            0, 1, 2,
            0, 2, 3,

            // back face
            4, 6, 5,
            4, 7, 6,

            // left face
            4, 5, 1,
            4, 1, 0,

            // right face
            3, 2, 6,
            3, 6, 7,

            // top face
            1, 5, 6,
            1, 6, 2,

            // bottom face
            4, 0, 3,
            4, 3, 7
        };;
    };
}