#include <maths/common/matrixhelper.h>

#include <DirectXMath.h>

DirectX::XMFLOAT4X4 PrioEngine::Maths::GetIdentity4x4()
{
    return DirectX::XMFLOAT4X4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}