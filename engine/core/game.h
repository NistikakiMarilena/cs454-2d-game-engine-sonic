#pragma once

#include <cstdint>
#include <functional>

class Game
{
public:
    using Action = std::function<void(void)>;
    using Pred = std::function<bool(void)>;

private:
    // Action  render;
    // Action  anim;
    // Action  input;
    // Action  ai;
    // Action  physics;
    // Action  destruct;
    // Action  collisions;
    // Action  user;

    // Pred    done;

    Action  pauseResume;
    bool    isPaused = false;
    uint64_t pauseTime = 0;

    void Invoke(const Action& f) { if (f) f(); }

public:
    // --- setters ---
    // void SetRender(const Action& f) { render = f; }
    // void SetAnimations(const Action& f) { anim = f; }
    // void SetInput(const Action& f) { input = f; }
    // void SetAI(const Action& f) { ai = f; }
    // void SetPhysics(const Action& f) { physics = f; }
    // void SetCommitDestructions(const Action& f) { destruct = f; }
    // void SetCollisionChecking(const Action& f) { collisions = f; }
    // void SetUserCode(const Action& f) { user = f; }

    // void SetDone(const Pred& f) { done = f; }

    // void Render(void) { Invoke(render); }
    // void ProgressAnimations(void) { Invoke(anim); }`
    // void Input(void) { Invoke(input); }
    // void AI(void) { Invoke(ai); }
    // void Physics(void) { Invoke(physics); }
    // void CollisionChecking(void) { Invoke(collisions); }
    // void CommitDestructions(void) { Invoke(destruct); }
    // void UserCode(void) { Invoke(user); }

    virtual void Initialize(void) = 0;
    virtual void Render(void) = 0; 
    virtual void ProgressAnimations(void) = 0; 
    virtual void Input(void) = 0; 
    virtual void AI(void) = 0; 
    virtual void Physics(void) = 0; 
    virtual void CollisionChecking(void) = 0; 
    virtual void CommitDestructions(void) = 0; 
    virtual void UserCode(void) = 0; 

    // If done() is not set, we treat game as "not finished"
    virtual bool IsFinished(void) const = 0;

    // --- pause API ---
    void SetOnPauseResume(const Action& f) { pauseResume = f; }
    void Pause(uint64_t t);
    void Resume(void);

    bool IsPaused(void) const { return isPaused; }
    uint64_t GetPauseTime(void) const { return pauseTime; }

    void MainLoop(void);
    void MainLoopIteration(void);
};
