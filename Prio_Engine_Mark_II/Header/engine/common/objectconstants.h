#pragma once

#include <maths/common/matrixhelper.h>

#include <DirectXMath.h>

namespace PrioEngine
{
    struct ObjectConstants
    {
        DirectX::XMFLOAT4X4 m_WorldViewProj = PrioEngine::Maths::GetIdentity4x4();
    };
}