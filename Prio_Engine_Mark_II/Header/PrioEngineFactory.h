#pragma once

#include <memory>
#include <Windows.h>
#include "D3D12PrioEngine.h"
#include "EEngineTypes.h"

namespace PrioEngineII
{
	class PrioEngineFactory
	{
	public:
		static std::shared_ptr<PrioEngine> CreateEngine(EEngineTypes engineType, HINSTANCE hInstance);
	};
}