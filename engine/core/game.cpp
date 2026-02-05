#include "game.h"

#include <chrono>
#include <thread>

// bool Game::IsFinished(void) const
// {
//     // If no predicate is provided, do not finish by default.
//     if (!done)
//         return false;

//     // done() == true means finished
//     return done();
// }

void Game::Pause(uint64_t t)
{
    isPaused = true;
    pauseTime = t;
    Invoke(pauseResume);
}

void Game::Resume(void)
{
    isPaused = false;
    Invoke(pauseResume);
    pauseTime = 0;
}

void Game::MainLoop(void)
{
    while (!IsFinished())
        MainLoopIteration();
}

void Game::MainLoopIteration(void)
{
    const auto frameStart = std::chrono::steady_clock::now();

    Render();
    Input();

    if (!IsPaused())
    {
        ProgressAnimations();
        AI();
        Physics();
        CollisionChecking();
        CommitDestructions();
        UserCode();
    }

    const auto frameEnd = std::chrono::steady_clock::now();
    const auto frameElapsed = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
    constexpr auto targetFrame = std::chrono::microseconds(16667); // ~60 FPS
    if (frameElapsed < targetFrame)
        std::this_thread::sleep_for(targetFrame - frameElapsed);
}
