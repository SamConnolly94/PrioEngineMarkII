#include "pch.h"
#include "PrioEngineFactory.h"
#include "EngineCreationException.h"

using namespace PrioEngineII;

std::shared_ptr<PrioEngine> PrioEngineFactory::CreateEngine(EEngineTypes engineType, HINSTANCE hInstance)
{
	switch (engineType)
	{
	case EEngineTypes::DX3D12:
		return D3D12PrioEngine::Factory(hInstance);
	default:
		throw EngineCreationException();
		break;
	}
}