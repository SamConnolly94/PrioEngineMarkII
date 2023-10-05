#include <prioengine.h>

using namespace std;

int main()
{
    //CPrioEngine engine{ EGraphicsAPI::DX12 };
    CPrioEngine* engine = new CPrioEngine(EGraphicsAPI::DX12);


    while (engine->Update())
    {
        // Game loop here
    }
}