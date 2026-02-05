#include "sonic.h"

#include "../engine/render/renderer.h"
#include "SDL3/SDL.h"

#include <algorithm>
#include <cctype>

namespace {
    const char* kDefaultAnim = "sonic_idle";
    const char* kIdleAnim = "sonic_bored";
    const char* kMoveAnim = "sonic_basic_motion";
    const char* kMoveAnimAngled = "sonic_basic_motion_angled";
    const char* kMoveAnimFast = "sonic_full_speed";
    const char* kMoveAnimFastAngled = "sonic_full_speed_angled";
    const char* kSkidAnim = "sonic_skidding";
    const char* kLookUpAnim = "sonic_looking_up";
    const char* kCurlAnim = "sonic_curling_up";

    int ClampInt(int value, int minValue, int maxValue)
    {
        if (value < minValue)
            return minValue;
        if (value > maxValue)
            return maxValue;
        return value;
    }

    bool TryParseIndex(const std::string& text, int& outIndex)
    {
        if (text.empty())
            return false;

        for (char c : text)
        {
            if (!std::isdigit(static_cast<unsigned char>(c)))
                return false;
        }

        outIndex = std::stoi(text);
        return true;
    }
}

void SonicGame::Initialize(void)
{
    config = core::config::LoadFromAssetsRoot(std::string(ASSETS));

    std::string spritePath;
    if (!config.player.image.empty())
        spritePath = std::string(ASSETS) + '/' + config.player.image;
    if (!spritePath.empty())
        spriteSheet = gfx::LoadBitmap(spritePath.c_str());

    if (!config.render.mapImage.empty())
    {
        const std::string renderPath = std::string(ASSETS) + '/' + config.render.mapImage;
        renderSheet = gfx::LoadBitmap(renderPath.c_str());
    }

    sceneBuffer = gfx::CreateBitmap(config.window.viewWidth, config.window.viewHeight);
    BuildMapLayer();

    playerX = config.player.startX;
    playerY = config.player.startY;
    playerSpeed = config.player.speed;
    playerIdleFrame = {
        config.player.idleFrame.x,
        config.player.idleFrame.y,
        config.player.idleFrame.w,
        config.player.idleFrame.h
    };
    if (playerIdleFrame.w > 0 && playerIdleFrame.h > 0)
    {
        playerW = playerIdleFrame.w;
        playerH = playerIdleFrame.h;
    }
    else
    {
        playerW = config.render.rectW;
        playerH = config.render.rectH;
        playerIdleFrame = {
            config.render.rectX,
            config.render.rectY,
            config.render.rectW,
            config.render.rectH
        };
    }

    BuildAnimations();
    if (currentAnim.empty())
        currentAnim = "sonic_bored";

    keepOpen = true;
}
void SonicGame::Render(void)
{
    if (!sceneBuffer)
        return;

    ClearSceneBuffer();
    UpdateCamera();
    BlitMapToScene();
    // BlitSpriteLayer(sceneBuffer);
    BlitAnimationLayer(sceneBuffer);

    RenderBitmap(sceneBuffer);
} 
void SonicGame::ProgressAnimations(void)
{
    StepAnimation();
} 
void SonicGame::Input(void)
{
    PollEvents();

    int moveX = 0;
    int moveY = 0;
    ReadMovementInput(moveX, moveY);
    ApplyMovement(moveX, moveY);
    UpdateAnimationState(moveX, moveY);
    ClampPlayerToMap();
} 
void SonicGame::AI(void)
{

} 
void SonicGame::Physics(void)
{

} 
void SonicGame::CollisionChecking(void)
{

} 
void SonicGame::CommitDestructions(void)
{

} 
void SonicGame::UserCode(void)
{

} 

bool SonicGame::IsFinished(void) const 
{
	return !keepOpen;
}

void SonicGame::BuildMapLayer(void)
{
    // Tile-based map (old approach)
    // if (!mapLayer || !spriteSheet)
    //     return;
    //
    // auto& cfg = config;
    // const int tileW = cfg.render.rectW;
    // const int tileH = cfg.render.rectH;
    // if (tileW <= 0 || tileH <= 0)
    //     return;
    //
    // gfx::ClearBitmap(mapLayer, 25, 25, 40, 255);
    //
    // const gfx::Rect tile = { cfg.render.rectX, cfg.render.rectY, tileW, tileH };
    // const int sceneW = config.window.viewWidth;
    // const int sceneH = config.window.viewHeight;
    //
    // for (int y = 0; y < sceneH; y += tileH)
    // {
    //     for (int x = 0; x < sceneW; x += tileW)
    //     {
    //         gfx::BlitBitmap(
    //             spriteSheet,
    //             tile,
    //             mapLayer,
    //             { x, y }
    //         );
    //     }
    // }

    if (config.window.mapImage.empty())
        return;

    const std::string mapPath = std::string(ASSETS) + '/' + config.window.mapImage;
    if (mapLayer)
    {
        gfx::DeleteBitmap(mapLayer);
        mapLayer = nullptr;
    }

    mapLayer = gfx::LoadBitmap(mapPath.c_str());
    if (!mapLayer)
        return;

    mapWidth = gfx::BitmapWidth(mapLayer);
    mapHeight = gfx::BitmapHeight(mapLayer);
	return;
}

void SonicGame::BuildAnimations(void)
{
    animations.clear();

    struct IndexedFrame {
        int index = 0;
        gfx::Rect rect = { 0, 0, 0, 0 };
    };

    std::unordered_map<std::string, std::vector<IndexedFrame>> indexed;

    for (const auto& kv : config.player.frames)
    {
        const std::string& name = kv.first;
        const auto& r = kv.second;
        const gfx::Rect rect = { r.x, r.y, r.w, r.h };

        const size_t lastUnderscore = name.find_last_of('_');
        int idx = 0;
        if (lastUnderscore != std::string::npos)
        {
            const std::string prefix = name.substr(0, lastUnderscore);
            const std::string suffix = name.substr(lastUnderscore + 1);
            if (TryParseIndex(suffix, idx))
            {
                indexed[prefix].push_back({ idx, rect });
                continue;
            }
        }

        animations[name].push_back(rect);
    }

    for (auto& kv : indexed)
    {
        auto& list = kv.second;
        std::sort(list.begin(), list.end(),
            [](const IndexedFrame& a, const IndexedFrame& b) { return a.index < b.index; });

        auto& out = animations[kv.first];
        out.reserve(list.size());
        for (const auto& f : list)
            out.push_back(f.rect);
    }

    if (animations.find(kDefaultAnim) == animations.end())
        animations[kDefaultAnim] = { playerIdleFrame };
}

void SonicGame::ClearSceneBuffer(void)
{
    gfx::ClearBitmap(sceneBuffer, 20, 80, 200, 255);
}

void SonicGame::BlitMapToScene(void)
{
    if (!mapLayer)
        return;

    const int viewW = config.window.viewWidth;
    const int viewH = config.window.viewHeight;

    int blitW = viewW;
    int blitH = viewH;
    if (mapWidth > 0 && mapWidth < blitW)
        blitW = mapWidth;
    if (mapHeight > 0 && mapHeight < blitH)
        blitH = mapHeight;

    gfx::BlitBitmap(
        mapLayer,
        { cameraX, cameraY, blitW, blitH },
        sceneBuffer,
        { 0, 0 }
    );
}

void SonicGame::PollEvents(void)
{
    auto& cfg = config;
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

void SonicGame::ReadMovementInput(int& moveX, int& moveY) const
{
    moveX = 0;
    moveY = 0;

    const bool* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_A]) moveX -= 1;
    if (keys[SDL_SCANCODE_D]) moveX += 1;
    if (keys[SDL_SCANCODE_W]) moveY -= 1;
    if (keys[SDL_SCANCODE_S]) moveY += 1;
}

void SonicGame::ApplyMovement(int moveX, int moveY)
{
    const int step = static_cast<int>(playerSpeed > 0.0f ? playerSpeed : 1.0f);
    playerX += moveX * step;
    playerY += moveY * step;
}

bool SonicGame::HasAnimation(const char* name) const
{
    if (!name)
        return false;

    auto it = animations.find(name);
    return it != animations.end() && !it->second.empty();
}

void SonicGame::UpdateAnimationState(int moveX, int moveY)
{
    const bool moving = (moveX != 0) || (moveY != 0);
    const bool diagonal = (moveX != 0) && (moveY != 0);
    const bool fast = playerSpeed >= 6.0f;
    const bool changedDir = (moveX != 0 && lastMoveX != 0 && moveX != lastMoveX);

    const char* next = nullptr;
    if (!moving)
    {
        if (HasAnimation(kIdleAnim)) next = kIdleAnim;
        else if (HasAnimation(kDefaultAnim)) next = kDefaultAnim;
    }
    else if (changedDir && HasAnimation(kSkidAnim))
    {
        next = kSkidAnim;
    }
    else if (moveY < 0 && moveX == 0 && HasAnimation(kLookUpAnim))
    {
        next = kLookUpAnim;
    }
    else if (moveY > 0 && moveX == 0 && HasAnimation(kCurlAnim))
    {
        next = kCurlAnim;
    }
    else if (diagonal)
    {
        if (fast && HasAnimation(kMoveAnimFastAngled)) next = kMoveAnimFastAngled;
        else if (HasAnimation(kMoveAnimAngled)) next = kMoveAnimAngled;
        else if (HasAnimation(kMoveAnim)) next = kMoveAnim;
    }
    else
    {
        if (fast && HasAnimation(kMoveAnimFast)) next = kMoveAnimFast;
        else if (HasAnimation(kMoveAnim)) next = kMoveAnim;
    }

    if (!next)
        return;

    if (currentAnim != next)
    {
        currentAnim = next;
        animFrame = 0;
        animTick = 0;
    }

    if (moveX != 0)
    {
        lastMoveX = moveX;
        facingLeft = (moveX < 0);
    }
    if (moveY != 0)
        lastMoveY = moveY;
}

void SonicGame::ClampPlayerToMap(void)
{
    if (mapWidth <= 0 || mapHeight <= 0)
        return;

    const int maxX = mapWidth - playerW;
    const int maxY = mapHeight - playerH;
    playerX = ClampInt(playerX, 0, maxX > 0 ? maxX : 0);
    playerY = ClampInt(playerY, 0, maxY > 0 ? maxY : 0);
}

const std::vector<gfx::Rect>* SonicGame::GetCurrentAnimFrames(void) const
{
    const std::string& key = currentAnim.empty() ? std::string(kDefaultAnim) : currentAnim;
    auto it = animations.find(key);
    if (it != animations.end() && !it->second.empty())
        return &it->second;

    auto idleIt = animations.find(kDefaultAnim);
    if (idleIt != animations.end() && !idleIt->second.empty())
        return &idleIt->second;

    return nullptr;
}

gfx::Rect SonicGame::GetCurrentAnimFrameRect(const std::vector<gfx::Rect>& frames) const
{
    if (frames.empty())
        return { 0, 0, 0, 0 };

    int frame = animFrame;
    const int maxFrames = static_cast<int>(frames.size());
    if (frame < 0 || frame >= maxFrames)
        frame = 0;
    return frames[frame];
}

void SonicGame::StepAnimation(void)
{
    const auto* frames = GetCurrentAnimFrames();
    if (!frames)
    {
        animFrame = 0;
        animTick = 0;
        return;
    }

    const int maxFrames = static_cast<int>(frames->size());
    if (animFrame >= maxFrames)
        animFrame = 0;

    if (maxFrames <= 1)
    {
        animFrame = 0;
        animTick = 0;
        return;
    }

    animTick++;
    if (animTick >= animFrameDelay)
    {
        animTick = 0;
        animFrame = (animFrame + 1) % maxFrames;
    }
}

gfx::Point SonicGame::WorldToScreen(int worldX, int worldY) const
{
    return { worldX - cameraX, worldY - cameraY };
}

void SonicGame::BlitPlayerFrame(gfx::Bitmap dest, const gfx::Rect& src, const gfx::Point& screenPos) const
{
    if (facingLeft)
    {
        gfx::BlitBitmapFlippedX(spriteSheet, src, dest, screenPos);
        return;
    }

    gfx::BlitBitmap(spriteSheet, src, dest, screenPos);
}

void SonicGame::UpdateCamera(void)
{
    const int viewW = config.window.viewWidth;
    const int viewH = config.window.viewHeight;

    if (mapWidth <= 0 || mapHeight <= 0 || viewW <= 0 || viewH <= 0)
    {
        cameraX = 0;
        cameraY = 0;
        return;
    }

    const int targetX = playerX + playerW / 2 - viewW / 2;
    const int targetY = playerY + playerH / 2 - viewH / 2;

    const int maxX = mapWidth - viewW;
    const int maxY = mapHeight - viewH;

    cameraX = ClampInt(targetX, 0, maxX > 0 ? maxX : 0);
    cameraY = ClampInt(targetY, 0, maxY > 0 ? maxY : 0);
}

void SonicGame::BlitSpriteLayer(gfx::Bitmap dest)
{
    if (!dest || !renderSheet)
        return;

    auto& cfg = config;
    if (cfg.render.drawPositions.empty())
        return;

    const gfx::Rect src = { cfg.render.rectX, cfg.render.rectY, cfg.render.rectW, cfg.render.rectH };

    for (const auto& p : cfg.render.drawPositions)
    {
        gfx::BlitBitmap(
            renderSheet,
            src,
            dest,
            { p.x - cameraX, p.y - cameraY }
        );
    }
}

void SonicGame::BlitAnimationLayer(gfx::Bitmap dest)
{
    if (!dest || !spriteSheet)
        return;

    const auto* frames = GetCurrentAnimFrames();
    if (!frames)
        return;

    const gfx::Rect src = GetCurrentAnimFrameRect(*frames);
    const gfx::Point screenPos = WorldToScreen(playerX, playerY);

    BlitPlayerFrame(dest, src, screenPos);
}

void SonicGame::RenderBitmap(gfx::Bitmap bitmap)
{
    if (!bitmap)
        return;

    const int w = gfx::BitmapWidth(bitmap);
    const int h = gfx::BitmapHeight(bitmap);
    if (w <= 0 || h <= 0)
        return;

    gfx::BlitBitmap(
        bitmap,
        { 0, 0, w, h },
        gfx::GetScreenBuffer(),
        { 0, 0 }
    );

    gfx::flush();
}
