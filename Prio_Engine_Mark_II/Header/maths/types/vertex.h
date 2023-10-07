#pragma once

#include <DirectXMath.h>

namespace PrioEngine
{
    namespace Math
    {
        struct Vertex
        {
            DirectX::XMFLOAT3 m_Position;
            DirectX::XMFLOAT4 m_Colour;
        };
    }
}