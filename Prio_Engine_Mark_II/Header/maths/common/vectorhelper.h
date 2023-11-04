#pragma once

#include <DirectXMath.h>

namespace PrioEngine
{
    namespace Math
    {
        struct Vector4;
        struct Vector3;

        DirectX::XMVECTOR ToXMVECTOR(const PrioEngine::Math::Vector3& vector);
        DirectX::XMVECTOR ToXMVECTOR(const PrioEngine::Math::Vector4& vector);
    }
}