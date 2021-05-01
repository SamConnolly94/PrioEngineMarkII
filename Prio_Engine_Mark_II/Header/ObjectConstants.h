#pragma once

#include <DirectXMath.h>
#include "d3dUtil.h"
using namespace DirectX;

namespace PrioEngineII
{
	struct ObjectConstants
	{
		XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	};
}