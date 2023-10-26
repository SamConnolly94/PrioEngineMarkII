#pragma once

#include <maths/vector3.h>
#include <engine/rendering/types/colour.h>

namespace PrioEngine
{
    namespace Maths
    {
        struct Vertex
        {
            PrioEngine::Maths::Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
            PrioEngine::Colour m_Colour{ 0.0f, 0.0f, 0.0f, 0.0f };
        };
    }
}