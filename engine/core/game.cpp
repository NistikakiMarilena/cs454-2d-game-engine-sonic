#include "game.h"

bool Game::IsFinished(void) const
{
    // If no predicate is provided, do not finish by default.
    if (!done)
        return false;

    // done() == true means finished
    return done();
}

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
}
