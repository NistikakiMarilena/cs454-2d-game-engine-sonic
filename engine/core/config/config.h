#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "SDL3/SDL.h"

namespace core::config {

    struct WindowConfig {
        std::string title;
        int width = 0;
        int height = 0;
        int viewWidth = 0;
        int viewHeight = 0;
        std::string mapImage; // relative to ASSETS
    };

    struct RenderConfig {
        std::string mapImage;    // relative to ASSETS
        int rectX = 0, rectY = 0, rectW = 0, rectH = 0;

        struct Point { int x = 0; int y = 0; };
        std::vector<Point> drawPositions;
    };

    struct InputConfig {
        SDL_Keycode quitKey = SDLK_ESCAPE;
    };

    struct AudioConfig {
        int freq = 44100;
        int channels = 2;
        float volume = 0.75f;
        std::string music; // relative to ASSETS
    };

    struct SpriteRect {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };

    struct PlayerConfig {
        int startX = 0;
        int startY = 0;
        float speed = 0.0f;
        std::string image; // relative to ASSETS
        SpriteRect idleFrame;
        std::unordered_map<std::string, SpriteRect> frames;
    };

    struct GameConfig {
        WindowConfig window;
        RenderConfig render;
        InputConfig input;
        AudioConfig audio;
        PlayerConfig player;
    };

    // Loads app/assets/config/game.json and then the referenced files.
    GameConfig LoadFromAssetsRoot(const std::string& assetsRoot,
        const std::string& gameJsonRelPath = "config/game.json");

} // namespace core::config
