#include "app.h"
#include "./config/config.h"
#include "audio/audio.h"

#include <stdexcept>

// #include "animator_manager.h"   // AnimatorManager::GetSingleton()
#include "time.h"               // GetGameTime() or wherever you declared it

namespace core {

void App::SetGame(Game* game)
{
    if (!game)
        throw std::invalid_argument("App::SetGame called with null Game*.");

    this->game = game;
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
        config.window.viewWidth,
        config.window.viewHeight
    );

    core::audio::Init(config.audio);
    if (!config.audio.music.empty())
    {
        const std::string musicPath = assetsPath + "/" + config.audio.music;
        backgroundMusic = core::audio::LoadMusic(musicPath.c_str());
        if (backgroundMusic)
            core::audio::PlayMusic(backgroundMusic, -1);
    }

    keepOpen = true;
    isInitialized = true;
}

void App::Shutdown()
{
    if (!isInitialized)
        return;

    if (backgroundMusic)
    {
        core::audio::StopMusic(backgroundMusic, 0);
        core::audio::DestroyMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    core::audio::Shutdown();
    gfx::Shutdown();
    isInitialized = false;
}

} // namespace core
