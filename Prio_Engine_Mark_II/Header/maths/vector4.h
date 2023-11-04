#pragma once

#include <ostream>

namespace PrioEngine
{
    namespace Math
    {
        struct Vector3;

        struct Vector4
        {
            Vector4(float x, float y, float z, float w) : m_X(x), m_Y(y), m_Z(z), m_W(w) {};
            Vector4(const Vector4& other) : m_X(other.m_X), m_Y(other.m_Y), m_Z(other.m_Z), m_W(other.m_W) {};
            Vector4(const Vector3& other);

            float m_X{ 0.0f };
            float m_Y{ 0.0f };
            float m_Z{ 0.0f };
            float m_W{ 0.0f };

            static float DotProduct(const Vector4& a, const Vector4& b);
            static Vector4 CrossProduct(const Vector4& a, const Vector4& b);
            static float Distance(const Vector4& a, const Vector4& b);
            static Vector4 Normalise(const Vector4& a);
            static Vector4 GetZero() { return Vector4{ 0.0f, 0.0f, 0.0f, 0.0f }; };

            float Length() const;
            void Normalise();

            // Operators
            Vector4& operator= (const Vector4& other);
            Vector4& operator*= (const float magnitude);
            bool operator== (const Vector4& other) const;
            Vector4& operator* (const float magnitude);
            Vector4& operator/ (const float magnitude);
            Vector4& operator- (const Vector4& other);
            Vector4& operator+ (const Vector4& other);
            friend std::ostream& operator<<(std::ostream& os, const Vector4& vector);
        };
        Vector4 operator* (const float magnitude, const Vector4& v);
        Vector4 operator/ (const float magnitude, const Vector4& v);
    }
}