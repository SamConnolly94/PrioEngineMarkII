#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    myEngine = CreateEngine(hInstance);

    // Game loop
    while (myEngine->IsRunning())
    {
        
        myEngine->DrawFrame();

    }
}

std::shared_ptr<PrioEngine> CreateEngine(const HINSTANCE& hInstance)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::shared_ptr<PrioEngine> myEngine = nullptr;
    try
    {
        myEngine = PrioEngineFactory::CreateEngine(EEngineTypes::DX3D12, hInstance);
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return nullptr;
    }

    if (myEngine->Initialise(myEngine) != EEngineCodes::Success)
    {
        return nullptr;
    }

    return std::move(myEngine);
}
