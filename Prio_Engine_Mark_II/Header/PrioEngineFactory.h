#pragma once

#include <memory>
#include <Windows.h>
#include "D3D12PrioEngine.h"

namespace PrioEngineII
{
	enum class EEngineTypes
	{
		DX3D12
	};

	class PrioEngineFactory
	{
	public:
		static std::shared_ptr<PrioEngine> CreateEngine(EEngineTypes engineType, HINSTANCE hInstance);
	};
}