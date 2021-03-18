#pragma once

namespace PrioEngineII
{
	enum EEngineCodes;

	class CPrioEngine
	{
	protected:
		CPrioEngine();

		virtual EEngineCodes GameLoop() = 0;
		virtual void Render() = 0;
	};
}