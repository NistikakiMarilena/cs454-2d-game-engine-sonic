#pragma once

#include "game.h"
#include "config/config.h"
#include "../render/bitmap.h"
#include "../render/renderer.h"

namespace core {

class App {
protected:
    Game game;

public:
    virtual ~App() = default;

    void Initialize(const std::string& assetsPath);
    void Load(void);
    void Run(void) { game.MainLoop(); }
    void RunIteration(void) { game.MainLoopIteration(); }
    void Clear(void);

    Game& GetGame(void) { return game; }
    const Game& GetGame(void) const { return game; }

    void SetGame(Game* game);

    //void Main(void) {
    //    Initialize();
    //    Load();
    //    Run();
    //    Clear();
    //}

private:
    bool isInitialized = false;
    bool keepOpen = false;

    config::GameConfig config;

    gfx::Bitmap testBitmap = nullptr;

    void InstallRenderHandler(Game& game);
    void InstallInputHandler(Game& game);
    void InstallDoneHandler(Game& game);
    void RenderBitmap(gfx::Bitmap bitmap);
};

// initially
void InstallPauseResumeHandler(Game& game);

// in response to the pause / resume input key
void HandleToglePauseResume(Game& game);

} // namespace core
