#include "config.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

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

    static inline std::string AssetsPath(const std::string& assetsRoot, const std::string& rel)
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
        out.sceneWidth = w.at("sceneWidth").get<int>();
        out.sceneHeight = w.at("sceneHeight").get<int>();
        return out;
    }

    static RenderConfig ParseRenderConfig(const json& r)
    {
        RenderConfig out;
        out.testImage = r.at("testImage").get<std::string>();

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

    struct ConfigPaths {
        std::string windowRel;
        std::string renderRel;
        std::string inputRel;
    };

    static ConfigPaths ReadConfigPaths(const json& game)
    {
        const auto& cfgs = game.at("configs");

        ConfigPaths paths;
        paths.windowRel = cfgs.at("window").get<std::string>();
        paths.renderRel = cfgs.at("render").get<std::string>();
        paths.inputRel = cfgs.at("input").get<std::string>();
        return paths;
    }

    GameConfig LoadFromAssetsRoot(const std::string& assetsRoot,
        const std::string& gameJsonRelPath)
    {
        const json game = LoadJsonFileOrThrow(AssetsPath(assetsRoot, gameJsonRelPath));
        const ConfigPaths paths = ReadConfigPaths(game);

        const json w = LoadJsonFileOrThrow(AssetsPath(assetsRoot, paths.windowRel));
        const json r = LoadJsonFileOrThrow(AssetsPath(assetsRoot, paths.renderRel));
        const json i = LoadJsonFileOrThrow(AssetsPath(assetsRoot, paths.inputRel));

        GameConfig out;
        out.window = ParseWindowConfig(w);
        out.render = ParseRenderConfig(r);
        out.input = ParseInputConfig(i);

        return out;
    }

} // namespace core::config
