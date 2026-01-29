#include "stateManager.h"

#include "render/renderer.h"
#include <stdexcept>
#include <string>
#include <core/config/config.cpp>

using namespace core::config;

void StateManager::Initialize()
{
    if (isInitialized)
        throw std::logic_error("StateManager::Initialize called twice.");

    std::string assetsPath = std::string(ASSETS);
    const std::string& gameJsonRelPath = "config/game.json";

    GameConfig cfg = LoadFromAssetsRoot(assetsPath, gameJsonRelPath);

    // TODO: use cfg.window.* to create the real SDL window when you add that code
    // For now you are using your gfx wrapper:
    gfx::Init();

    std::string fullImagePath = AssetsPath(assetsPath, cfg.render.testImage);
    testBitmap = gfx::LoadBitmap(fullImagePath.c_str());

    keepOpen = true;
    isInitialized = true;
}

void StateManager::Run()
{
    EnsureInitialized("Run");

    while (keepOpen)
    {
        HandleEvents();
        RenderFrame();
        gfx::flush();
    }
}

void StateManager::Shutdown()
{
    if (!isInitialized)
        return;

    if (testBitmap)
    {
        gfx::DeleteBitmap(testBitmap);
        testBitmap = nullptr;
    }

    gfx::Shutdown();
    isInitialized = false;
}

void StateManager::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
            keepOpen = false;

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            if (event.key.key == cfg.input.quitKey)
                keepOpen = false;
        }
    }
}

void StateManager::RenderFrame()
{
    if (!testBitmap)
        return;

    // Use rect + positions from config
    gfx::Rect rect{ cfg.render.rectX, cfg.render.rectY, cfg.render.rectW, cfg.render.rectH };

    for (const auto& p : cfg.render.drawPositions)
    {
        gfx::BlitBitmap(
            testBitmap,
            rect,
            gfx::GetScreenBuffer(),
            { p.x, p.y }
        );
    }
}

void StateManager::EnsureInitialized(const char* caller) const
{
    if (!isInitialized)
        throw std::logic_error(std::string("StateManager::") + caller + " called before Initialize.");
}
