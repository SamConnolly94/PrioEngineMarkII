#include <pch.h>
#include <maths/common/vectorhelper.h>

#include <maths/vector4.h>
#include <maths/vector3.h>

namespace PrioEngine
{
    namespace Math
    {
        DirectX::XMVECTOR ToXMVECTOR(const PrioEngine::Math::Vector3& vector)
        {
            return DirectX::XMVECTOR(vector.m_X, vector.m_Y, vector.m_Z);
        }

        DirectX::XMVECTOR ToXMVECTOR(const PrioEngine::Math::Vector4& vector)
        {
            return DirectX::XMVECTOR(vector.m_X, vector.m_Y, vector.m_Z, vector.m_W);
        }
    }
}