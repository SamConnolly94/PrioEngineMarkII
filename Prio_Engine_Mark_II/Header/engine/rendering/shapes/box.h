#pragma once

#include <array>

struct Vertex;

namespace PrioEngine
{
    class CBox
    {
    public:
        CBox();
    private:
        std::array<Vertex, 8> m_Vertices;
    };
}