#include <pch.h>
#include <maths/vector2.h>

namespace PrioEngine
{
    namespace Maths
    {
        Vector2& Vector2::operator=(const Vector2& other)
        {
            return Vector2(other.m_X, other.m_Y);
        }

        Vector2& Vector2::operator*(const float magnitude)
        {
            return Vector2(m_X * magnitude, m_Y * magnitude);
        }

        Vector2& Vector2::operator-(const Vector2& other)
        {
            return Vector2(m_X - other.m_X, m_Y - other.m_Y);
        }

        Vector2& Vector2::operator+(const Vector2& other)
        {
            return Vector2(m_X + other.m_X, m_Y + other.m_Y);
        }

        Vector2 operator*(const float magnitude, const Vector2& v)
        {
            return Vector2(v.m_X * magnitude, v.m_Y * magnitude);
        }
    }
}