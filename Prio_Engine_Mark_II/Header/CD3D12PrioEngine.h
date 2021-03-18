#pragma once
#include "CPrioEngine.h"

namespace PrioEngineII
{
    enum EEngineCodes;

    class CD3D12PrioEngine :
        public CPrioEngine
    {
        // Inherited via CPrioEngine
        virtual EEngineCodes GameLoop() override;
        virtual void Render() override;
    };
}
