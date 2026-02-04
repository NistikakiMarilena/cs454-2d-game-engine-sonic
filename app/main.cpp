#include "../engine/core/app.h"
#include "sonic.h"
//#include "../engine/core/config/config.h"

int main()
{
    // std::string config_file = std::string(ASSETS) + '/' + "config/game.json";

    core::App App;
    App.SetGame(new app::SonicGame());
    App.Initialize(std::string(ASSETS));
    App.Run();
    App.Clear();
    return 0;
}
