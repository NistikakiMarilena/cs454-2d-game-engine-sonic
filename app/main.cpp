#include "bitmap.h"
#include "SDL3/SDL.h"

#include <string>

int main(void)
{
    bool keepOpen = true;
    std::string path(ASSETS);
    std::string png = path + "/images/a.png";

    gfx::Init();

    gfx::Bitmap a = gfx::LoadBitmap(png.c_str());
    
    while (keepOpen)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
			if (event.type == SDL_EVENT_QUIT)
				keepOpen = false;
            if (event.type == SDL_EVENT_KEY_DOWN)
                if (event.key.key == SDLK_ESCAPE)
                    keepOpen = false;
        }

        gfx::BlitBitmap(
            a,
            { 0, 0, 500, 500 },
            gfx::GetScreenBuffer(),
            { 0, 0 }
        );

        gfx::BlitBitmap(
            a,
            { 0, 0, 500, 500 },
            gfx::GetScreenBuffer(),
            { 500, 0 }
        );

        gfx::flush();
    }
    
    gfx::DeleteBitmap(a);

    gfx::Shudown();

    return 0;
}