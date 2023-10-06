#include <prioengine.h>

using namespace std;

int main()
{
    CPrioEngine::CreateInstance(EGraphicsAPI::DX12);
    CPrioEngine& engine = CPrioEngine::GetInstance();
    
    while (engine.Update())
    {
        // Game loop here
    }
}