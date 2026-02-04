#pragma once
#include <string>
#include <vector>
#include "SDL3/SDL.h"

namespace core::config {

    struct WindowConfig {
        std::string title;
        int width = 0;
        int height = 0;
        int sceneWidth = 0;
        int sceneHeight = 0;
    };

    struct RenderConfig {
        std::string testImage;   // relative to ASSETS
        int rectX = 0, rectY = 0, rectW = 0, rectH = 0;

        struct Point { int x = 0; int y = 0; };
        std::vector<Point> drawPositions;
    };

    struct InputConfig {
        SDL_Keycode quitKey = SDLK_ESCAPE;
    };

    struct GameConfig {
        WindowConfig window;
        RenderConfig render;
        InputConfig input;
    };

    struct AudioConfig {
        int freq = 44100;
        int channels = 2;
        float volume = 0.75f;
    };

    // Loads app/assets/config/game.json and then the referenced files.
    GameConfig LoadFromAssetsRoot(const std::string& assetsRoot,
        const std::string& gameJsonRelPath = "config/game.json");

} // namespace core::config
