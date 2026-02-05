#pragma once

#include "../engine/core/game.h"
#include "../engine/core/config/config.h"
#include "../engine/render/bitmap.h"

#include <string>
#include <unordered_map>
#include <vector>

class SonicGame : public Game {
public:
	void Initialize(void) override;
    void Render(void) override; 
    void ProgressAnimations(void) override; 
    void Input(void) override; 
    void AI(void) override; 
    void Physics(void) override; 
    void CollisionChecking(void) override; 
    void CommitDestructions(void) override; 
    void UserCode(void) override;
    bool IsFinished(void) const override;

private:
	int m_kappa;
    bool keepOpen = false;
    core::config::GameConfig config;
    gfx::Bitmap spriteSheet = nullptr;
    gfx::Bitmap renderSheet = nullptr;
    gfx::Bitmap mapLayer = nullptr;
    gfx::Bitmap sceneBuffer = nullptr;
    int mapWidth = 0;
    int mapHeight = 0;
    int cameraX = 0;
    int cameraY = 0;

    int animTick = 0;
    int animFrame = 0;
    int animFrameDelay = 6;
    int playerX = 0;
    int playerY = 0;
    float playerSpeed = 0.0f;
    int playerW = 0;
    int playerH = 0;
    gfx::Rect playerIdleFrame = { 0, 0, 0, 0 };
    std::unordered_map<std::string, std::vector<gfx::Rect>> animations;
    std::string currentAnim;
    int lastMoveX = 0;
    int lastMoveY = 0;

    void BuildMapLayer(void);
    void BuildAnimations(void);
    void ClearSceneBuffer(void);
    void BlitMapToScene(void);
    void PollEvents(void);
    void ReadMovementInput(int& moveX, int& moveY) const;
    void ApplyMovement(int moveX, int moveY);
    void UpdateAnimationState(int moveX, int moveY);
    bool HasAnimation(const char* name) const;
    void ClampPlayerToMap(void);
    void StepAnimation(void);
    const std::vector<gfx::Rect>* GetCurrentAnimFrames(void) const;
    gfx::Rect GetCurrentAnimFrameRect(const std::vector<gfx::Rect>& frames) const;
    gfx::Point WorldToScreen(int worldX, int worldY) const;
    void UpdateCamera(void);
    void BlitSpriteLayer(gfx::Bitmap dest);
    void BlitAnimationLayer(gfx::Bitmap dest);
    void RenderBitmap(gfx::Bitmap bitmap);
};
