#pragma once

#include <array>

namespace PrioEngine
{
    namespace Maths
    {
        struct Vertex;
    }

    class CBox
    {
    public:
        CBox();
    private:
        std::array<Maths::Vertex, 8> m_Vertices;
        std::array<std::uint16_t, 36> m_Indices;
    };
}