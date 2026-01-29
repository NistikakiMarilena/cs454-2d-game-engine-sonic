#include <gameStates/stateManager.h>

int main()
{
    StateManager stateManager;
    stateManager.Initialize();
    stateManager.Run();
    stateManager.Shutdown();
    return 0;
}
