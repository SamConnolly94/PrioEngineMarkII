#ifdef _DEBUG
#pragma comment(lib, "dprioengine2.dll")
#else
#pragma comment(lib, "prioengine2.dll")
#endif

#include <prioengine.h>
#include <shapes/box.h>

using namespace std;

int main()
{
    CPrioEngine::CreateInstance(EGraphicsAPI::DX12);
    CPrioEngine& engine = CPrioEngine::GetInstance();

    while (engine.Update())
    {
        // Game loop here
    }


    //delete(box);
}