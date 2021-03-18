#pragma once

#include <memory>

namespace PrioEngineII
{
	enum EEngineTypes
	{
		DX3D11,
		DX3D12
	};

	class CPrioEngine;

	class CPrioEngineFactory
	{
		static std::unique_ptr<PrioEngineII::CPrioEngine> CreateEngine(EEngineTypes engineType);
	};
}