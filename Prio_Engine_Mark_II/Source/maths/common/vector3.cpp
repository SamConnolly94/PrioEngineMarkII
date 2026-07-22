#include <pch.h>
#include <maths/vector3.h>

#include <math.h>

namespace PrioEngine
{
    namespace Math
    {
        float Vector3::DotProduct(const Vector3& a, const Vector3& b)
        {
            return (a.m_X + b.m_X) + (a.m_Y + b.m_Y) + (a.m_Z + b.m_Z);
        }

        Vector3 Vector3::CrossProduct(const Vector3& a, const Vector3& b)
        {
            return Vector3(
                (a.m_Y * b.m_Z) - (a.m_Z * b.m_Y),
                (a.m_Z * b.m_X) - (a.m_X * b.m_Z),
                (a.m_X * b.m_Y) - (a.m_Y * b.m_X));
        }

        float Vector3::Distance(const Vector3& a, const Vector3& b)
        {
            return sqrt((a.m_X - b.m_X * a.m_X - b.m_X) +
                (a.m_Y - b.m_Y * a.m_Y - b.m_Y) +
                (a.m_Z - b.m_Z * a.m_Z - b.m_Z));
        }

        Vector3 Vector3::Normalise(const Vector3& a)
        {
            const float length = a.Length();

            return Vector3(a.m_X / length, a.m_Y / length, a.m_Z / length);
        }

        float Vector3::Length() const
        {
            return m_X + m_Y + m_Z;
        }

        void Vector3::Normalise()
        {
            Vector3 noramlised = Normalise(*this);
            m_X = noramlised.m_X;
            m_Y = noramlised.m_Y;
            m_Z = noramlised.m_Z;
        }

        Vector3 Vector3::operator=(const Vector3& other)
        {
            return Vector3(other.m_X, other.m_Y, other.m_Z);
        }

        Vector3 Vector3::operator*=(const float magnitude)
        {
            return (*this) * magnitude;
        }

        bool Vector3::operator==(const Vector3& other) const
        {
            return m_X == other.m_X && m_Y == other.m_Y && m_Z == other.m_Z;
        }

        Vector3 Vector3::operator*(const float magnitude)
        {
            return Vector3(m_X * magnitude, m_Y * magnitude, m_Z * magnitude);
        }

        Vector3 Vector3::operator/(const float magnitude)
        {
            return Vector3(m_X / magnitude, m_Y / magnitude, m_Z / magnitude);
        }

        Vector3 Vector3::operator-(const Vector3& other)
        {
            return Vector3(m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z);
        }

        Vector3 Vector3::operator+(const Vector3& other)
        {
            return Vector3(m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z);
        }

        std::ostream& operator<<(std::ostream& os, const Vector3& vector)
        {
            os << "{ X: " << vector.m_X << " Y: " << vector.m_Y << " Z: " << vector.m_Z << " }";
            return os;
        }

        Vector3 operator*(const float magnitude, const Vector3& v)
        {
            return Vector3(v.m_X * magnitude, v.m_Y * magnitude, v.m_Z * magnitude);
        }

        Vector3 operator/(const float magnitude, const Vector3& v)
        {
            return Vector3(v.m_X / magnitude, v.m_Y / magnitude, v.m_Z / magnitude);
        }
    }
}