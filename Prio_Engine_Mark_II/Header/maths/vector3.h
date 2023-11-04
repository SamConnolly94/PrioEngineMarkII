#pragma once

#include <ostream>

namespace PrioEngine
{
    namespace Math
    {
        struct Vector3
        {
            Vector3(float x, float y, float z) : m_X(x), m_Y(y), m_Z(z) {};
            Vector3(const Vector3& other) : m_X(other.m_X), m_Y(other.m_Y), m_Z(other.m_Z) {};

            float m_X{ 0.0f };
            float m_Y{ 0.0f };
            float m_Z{ 0.0f };

            static float DotProduct(const Vector3& a, const Vector3& b);
            static Vector3 CrossProduct(const Vector3& a, const Vector3& b);
            static float Distance(const Vector3& a, const Vector3& b);
            static Vector3 Normalise(const Vector3& a);

            float Length() const;
            void Normalise();

            // Operators
            Vector3& operator= (const Vector3& other);
            Vector3& operator*= (const float magnitude);
            bool operator== (const Vector3& other) const;
            Vector3& operator* (const float magnitude);
            Vector3& operator/ (const float magnitude);
            Vector3& operator- (const Vector3& other);
            Vector3& operator+ (const Vector3& other);
            friend std::ostream& operator<<(std::ostream& os, const Vector3& vector);
        };

        Vector3 operator* (const float magnitude, const Vector3& v);
        Vector3 operator/ (const float magnitude, const Vector3& v);
    }
}