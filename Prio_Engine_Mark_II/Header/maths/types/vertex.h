#pragma once

#include <maths/vector3.h>

namespace PrioEngine
{
    namespace Maths
    {
        struct Vertex
        {
            PrioEngine::Maths::Vector3 m_Position;
            DirectX::XMFLOAT4 m_Colour;
        };
    }
}