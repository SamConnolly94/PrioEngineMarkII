#include "pch.h"
#include "CPrioEngineFactory.h"
#include "EngineCreationException.h"

using namespace PrioEngineII;

std::unique_ptr<PrioEngineII::CPrioEngine> CPrioEngineFactory::CreateEngine(EEngineTypes engineType)
{
	switch (engineType)
	{
	case PrioEngineII::DX3D11:
		throw EngineCreationException;
		break;
	case PrioEngineII::DX3D12:
		throw EngineCreationException;
		break;
	default:
		throw EngineCreationException;
		break;
	}
}