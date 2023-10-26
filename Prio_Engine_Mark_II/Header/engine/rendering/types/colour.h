#pragma once

// TODO:
// Need to separate out the dependency somehow
// Maybe move to a DirecXColour struct and have the constructor in there?
#include <DirectXColors.h>

namespace PrioEngine
{
    struct Colour
    {
        Colour(DirectX::XMVECTORF32 colour) : m_R(colour[0]), m_G(colour[1]), m_B(colour[2]), m_A(colour[3]) {};
        Colour(float r, float g, float b, float a) : m_R(r), m_G(g), m_B(b), m_A(a) {};

        float m_R{ 0.0f };
        float m_G{ 0.0f };
        float m_B{ 0.0f };
        float m_A{ 0.0f };
    };
}