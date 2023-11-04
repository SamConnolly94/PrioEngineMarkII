#pragma once

#include <maths/vector3.h>
#include <engine/rendering/types/colour.h>

namespace PrioEngine
{
    namespace Math
    {
        struct Vertex
        {
            Vertex(const PrioEngine::Math::Vector3& position, const PrioEngine::Colour& colour) : m_Position(position), m_Colour(colour) {};
            PrioEngine::Math::Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
            PrioEngine::Colour m_Colour{ 0.0f, 0.0f, 0.0f, 0.0f };
        };
    }
}