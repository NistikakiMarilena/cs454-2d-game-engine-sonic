#pragma once
#include <string>
#include "render/bitmap.h"
#include "SDL3/SDL.h"
#include "core/config/config.h"

class StateManager
{
public:
    StateManager() = default;
    ~StateManager() = default;

    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;
    StateManager(StateManager&&) = delete;
    StateManager& operator=(StateManager&&) = delete;

    void Initialize();
    void Run();
    void Shutdown();

private:
    bool isInitialized = false;
    bool keepOpen = false;

    core::config::GameConfig cfg;

    gfx::Bitmap testBitmap = nullptr;

    void HandleEvents();
    void RenderFrame();
    void EnsureInitialized(const char* caller) const;
};
