#include "../engine/core/app.h"
#include "sonic.h"

int main()
{
    core::App App;
    auto* game = new SonicGame();
    App.SetGame(game);
    App.Initialize(std::string(ASSETS));
    game->Initialize();
    App.Run();
    App.Shutdown();
    delete game;
    return 0;
}
