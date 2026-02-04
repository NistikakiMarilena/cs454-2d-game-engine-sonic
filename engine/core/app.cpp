#include "app.h"
#include "./config/config.h"

#include <stdexcept>

// #include "animator_manager.h"   // AnimatorManager::GetSingleton()
#include "time.h"               // GetGameTime() or wherever you declared it

namespace core {

void App::SetGame(Game* game)
{
    if (!game)
        throw std::invalid_argument("App::SetGame called with null Game*.");

    this->game = *game;

    InstallRenderHandler(this->game);
    InstallInputHandler(this->game);
    InstallDoneHandler(this->game);
    InstallPauseResumeHandler(this->game);
}

void App::InstallRenderHandler(Game& game)
{
    game.SetRender([this]() {
        RenderBitmap(testBitmap);
    });
}

void App::InstallInputHandler(Game& game)
{
    auto& cfg = config;
    game.SetInput([this, &cfg]() {
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
    });
}

void App::InstallDoneHandler(Game& game)
{
    game.SetDone([this]() { return !keepOpen; });
}

void App::RenderBitmap(gfx::Bitmap bitmap)
{
    if (!bitmap)
        return;

    auto& cfg = config;
    if (cfg.render.drawPositions.empty())
        return;

    for (const auto& p : cfg.render.drawPositions)
    {
        gfx::BlitBitmap(
            bitmap,
            { 0, 0, gfx::BitmapWidth(bitmap), gfx::BitmapHeight(bitmap) },
            gfx::GetScreenBuffer(),
            { p.x, p.y }
        );
    }

    gfx::flush();
}

void App::Initialize(const std::string& assetsPath)
{
    if (isInitialized)
        throw std::logic_error("App::Initialize called twice.");

    config = core::config::LoadFromAssetsRoot(assetsPath);

    gfx::Init(
        config.window.title.c_str(),
        config.window.width,
        config.window.height,
        config.window.sceneWidth,
        config.window.sceneHeight
    );

    // init audio


    std::string fullImagePath = assetsPath + '/' + config.render.testImage;
    testBitmap = gfx::LoadBitmap(fullImagePath.c_str());

    keepOpen = true;
    isInitialized = true;
}

void App::Load() {
    return;
}

void App::Clear()
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


void InstallPauseResumeHandler(Game& game)
{
    game.SetOnPauseResume(
        [&game](void) {
            //if (!game.IsPaused()) // just resumed
                // AnimatorManager::GetSingleton().TimeShift(
                //     GetGameTime() - game.GetPauseTime()
                // );
        }
    );
}

void HandleToglePauseResume(Game& game)
{
    if (game.IsPaused())
        game.Resume();
    //else
        // game.Pause(GetGameTime());
}

} // namespace core
