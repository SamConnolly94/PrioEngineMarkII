#include <pch.h>
#include <maths/vector4.h>

#include <maths/vector3.h>

#include <math.h>

namespace PrioEngine
{
    namespace Math
    {
        Vector4::Vector4(const Vector3& other) : m_X(other.m_X), m_Y(other.m_Y), m_Z(other.m_Z) {}

        float Vector4::DotProduct(const Vector4& a, const Vector4& b)
        {
            return (a.m_X * b.m_X) + (a.m_Y * b.m_Y) + (a.m_Z * b.m_Z) + (a.m_W * b.m_W);
        }

        Vector4 Vector4::CrossProduct(const Vector4& a, const Vector4& b)
        {
            // TODO:
            // Is this right for dot prod of a vec 4? Pretty sure there's one more calc to do
            return Vector4(
                (a.m_Y * b.m_Z) - (a.m_Z * b.m_Y),
                (a.m_Z * b.m_X) - (a.m_X * b.m_Z),
                (a.m_X * b.m_Y) - (a.m_Y * b.m_X),
                0.0f);
        }

        float Vector4::Distance(const Vector4& a, const Vector4& b)
        {
            return sqrt((a.m_X - b.m_X * a.m_X - b.m_X) +
                (a.m_Y - b.m_Y * a.m_Y - b.m_Y) +
                (a.m_Z - b.m_Z * a.m_Z - b.m_Z) + 
                (a.m_W - b.m_W * a.m_W - b.m_W));
        }

        Vector4 Vector4::Normalise(const Vector4& a)
        {
            const float length = a.Length();

            return Vector4(a.m_X / length, a.m_Y / length, a.m_Z / length, a.m_W / length);
        }

        float Vector4::Length() const
        {
            return m_X + m_Y + m_Z + m_W;
        }

        void Vector4::Normalise()
        {
            Vector4 noramlised = Normalise(*this);
            m_X = noramlised.m_X;
            m_Y = noramlised.m_Y;
            m_Z = noramlised.m_Z;
            m_W = noramlised.m_W;
        }

        Vector4& Vector4::operator=(const Vector4& other)
        {
            return Vector4(other.m_X, other.m_Y, other.m_Z, other.m_W);
        }

        Vector4& Vector4::operator*=(const float magnitude)
        {
            return (*this) * magnitude;
        }

        bool Vector4::operator==(const Vector4& other) const
        {
            return m_X == other.m_X && m_Y == other.m_Y && m_Z == other.m_Z && m_W == other.m_W;
        }

        Vector4& Vector4::operator*(const float magnitude)
        {
            return Vector4(m_X * magnitude, m_Y * magnitude, m_Z * magnitude, m_W * magnitude);
        }

        Vector4& Vector4::operator/(const float magnitude)
        {
            return Vector4(m_X / magnitude, m_Y / magnitude, m_Z / magnitude, m_W / magnitude);
        }

        Vector4& Vector4::operator-(const Vector4& other)
        {
            return Vector4(m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z, m_W - other.m_W);
        }

        Vector4& Vector4::operator+(const Vector4& other)
        {
            return Vector4(m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z, m_W + other.m_W);
        }

        std::ostream& operator<<(std::ostream& os, const Vector4& vector)
        {
            os << "{ X: " << vector.m_X << " Y: " << vector.m_Y << " Z: " << vector.m_Z << " W: " << vector.m_W << " }";
            return os;
        }

        Vector4 operator*(const float magnitude, const Vector4& v)
        {
            return Vector4(v.m_X * magnitude, v.m_Y * magnitude, v.m_Z * magnitude, v.m_W * magnitude);
        }

        Vector4 operator/(const float magnitude, const Vector4& v)
        {
            return Vector4(v.m_X / magnitude, v.m_Y / magnitude, v.m_Z / magnitude, v.m_W / magnitude);
        }

    }
}