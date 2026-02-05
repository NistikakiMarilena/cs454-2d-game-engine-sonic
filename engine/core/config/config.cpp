#include "config.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <iostream>

using json = nlohmann::json;

namespace core::config {

    static json LoadJsonFileOrThrow(const std::string& fullPath)
    {
        std::ifstream f(fullPath);
        if (!f)
            throw std::runtime_error("Failed to open JSON file: " + fullPath);

        json j;
        f >> j;
        return j;
    }

    inline std::string AssetsPath(const std::string& assetsRoot, const std::string& rel)
    {
        return assetsRoot + "/" + rel;
    }


    static SDL_Keycode ParseKeyOrDefault(const std::string& s, SDL_Keycode defKey)
    {
        // Expand as you add more bindings.
        if (s == "Escape") return SDLK_ESCAPE;
        if (s == "Space")  return SDLK_SPACE;
        if (s == "Return") return SDLK_RETURN;
        if (s == "Left")   return SDLK_LEFT;
        if (s == "Right")  return SDLK_RIGHT;
        if (s == "Up")     return SDLK_UP;
        if (s == "Down")   return SDLK_DOWN;
        return defKey;
    }

    static WindowConfig ParseWindowConfig(const json& w)
    {
        WindowConfig out;
        out.title = w.at("title").get<std::string>();
        out.width = w.at("width").get<int>();
        out.height = w.at("height").get<int>();
        if (w.contains("viewWidth"))
            out.viewWidth = w.at("viewWidth").get<int>();
        else
            out.viewWidth = w.at("sceneWidth").get<int>();

        if (w.contains("viewHeight"))
            out.viewHeight = w.at("viewHeight").get<int>();
        else
            out.viewHeight = w.at("sceneHeight").get<int>();
        out.mapImage = w.value("mapImage", std::string());
        return out;
    }

    static RenderConfig ParseRenderConfig(const json& r)
    {
        RenderConfig out;
        out.mapImage = r.value("mapImage", std::string());

        const auto& rect = r.at("spriteRect");
        out.rectX = rect.at("x").get<int>();
        out.rectY = rect.at("y").get<int>();
        out.rectW = rect.at("w").get<int>();
        out.rectH = rect.at("h").get<int>();

        out.drawPositions.clear();
        for (const auto& p : r.at("drawPositions"))
            out.drawPositions.push_back({ p.at("x").get<int>(), p.at("y").get<int>() }); 

        return out;
    }

    static InputConfig ParseInputConfig(const json& i)
    {
        InputConfig out;
        const std::string quitKey = i.at("quitKey").get<std::string>();
        out.quitKey = ParseKeyOrDefault(quitKey, SDLK_ESCAPE);
        return out;
    }

    static AudioConfig ParseAudioConfig(const json& a)
    {
        AudioConfig out;
        out.freq = a.value("freq", out.freq);
        out.channels = a.value("channels", out.channels);
        out.volume = a.value("volume", out.volume);
        out.music = a.value("music", out.music);
        return out;
    }

    static PlayerConfig ParsePlayerConfig(const json& p)
    {
        PlayerConfig out;
        out.startX = p.value("startX", out.startX);
        out.startY = p.value("startY", out.startY);
        out.speed = p.value("speed", out.speed);
        out.image = p.value("image", out.image);

        if (p.contains("frames"))
        {
            const auto& frames = p.at("frames");
            for (auto it = frames.begin(); it != frames.end(); ++it)
            {
                const std::string name = it.key();
                const auto& f = it.value();
                out.frames[name] = {
                    f.value("x", 0),
                    f.value("y", 0),
                    f.value("w", 0),
                    f.value("h", 0)
                };
            }
        }
        if (out.frames.count("idle"))
            out.idleFrame = out.frames["idle"];
        else if (out.frames.count("sonic_idle"))
            out.idleFrame = out.frames["sonic_idle"];
        return out;
    }

    struct ConfigPaths {
        std::string windowRel;
        std::string renderRel;
        std::string inputRel;
        std::string audioRel;
        std::string playerRel;
    };

    static ConfigPaths ReadConfigPaths(const json& game)
    {
        const auto& cfgs = game.at("configs");

        ConfigPaths paths;
        paths.windowRel = cfgs.at("window").get<std::string>();
        paths.renderRel = cfgs.at("render").get<std::string>();
        paths.inputRel = cfgs.at("input").get<std::string>();
        if (cfgs.contains("audio"))
            paths.audioRel = cfgs.at("audio").get<std::string>();
        if (cfgs.contains("player"))
            paths.playerRel = cfgs.at("player").get<std::string>();
        return paths;
    }

    GameConfig LoadFromAssetsRoot(const std::string& assetsRoot,
        const std::string& gameJsonRelPath)
    {
        const json game = LoadJsonFileOrThrow(AssetsPath(assetsRoot, gameJsonRelPath));
        const ConfigPaths paths = ReadConfigPaths(game);

        std::string path = AssetsPath(assetsRoot, paths.windowRel);
        const json w = LoadJsonFileOrThrow(path);
        path = AssetsPath(assetsRoot, paths.renderRel);
        const json r = LoadJsonFileOrThrow(path);
        path = AssetsPath(assetsRoot, paths.inputRel);
        const json i = LoadJsonFileOrThrow(path);

        GameConfig out;
        out.window = ParseWindowConfig(w);
        out.render = ParseRenderConfig(r);
        out.input = ParseInputConfig(i);
        if (!paths.audioRel.empty())
        {
            path = AssetsPath(assetsRoot, paths.audioRel);
            const json a = LoadJsonFileOrThrow(path);
            out.audio = ParseAudioConfig(a);
        }
        if (!paths.playerRel.empty())
        {
            path = AssetsPath(assetsRoot, paths.playerRel);
            const json p = LoadJsonFileOrThrow(path);
            out.player = ParsePlayerConfig(p);
        }

        return out;
    }

} // namespace core::config
