#pragma once

namespace PrioEngine
{
    namespace Math
    {
        struct Vector2
        {
            Vector2(float x, float y) : m_X(x), m_Y(y) {};

            float m_X{ 0.0f };
            float m_Y{ 0.0f };

            Vector2 operator= (const Vector2& other);
            Vector2 operator* (const float magnitude);
            Vector2 operator- (const Vector2& other);
            Vector2 operator+ (const Vector2& other);
        };

        Vector2 operator* (const float magnitude, const Vector2& v);
    }
}