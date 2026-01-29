#include "renderer.h"

#include "SDL3/SDL.h"
#include "stb_image.h"

SDL_Window* g_pWindow = nullptr;
SDL_Renderer* g_pRenderer = nullptr;

SDL_Texture* g_pBackBuffer = nullptr;
gfx::Bitmap g_pScreenBuffer = nullptr;

namespace gfx
{
	void Init()
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD);

		SDL_WindowFlags flags = { 0 };
		flags |= SDL_WINDOW_INPUT_FOCUS;
		flags |= SDL_WINDOW_MOUSE_CAPTURE;

		SDL_CreateWindowAndRenderer(
			"DEEEEEMYYYY",
			1024,
			768,
			flags,
			&g_pWindow,
			&g_pRenderer
		);

		g_pScreenBuffer = CreateBitmap(1024, 768);

		g_pBackBuffer = SDL_CreateTexture(
			g_pRenderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STREAMING,
			1024,
			768
		);
	}

	void Shutdown()
	{
		DeleteBitmap(g_pScreenBuffer);
		SDL_DestroyRenderer(g_pRenderer);
		SDL_DestroyWindow(g_pWindow);
		SDL_Quit();
	}

	Bitmap GetScreenBuffer()
	{
		return g_pScreenBuffer;
	}

	void flush()
	{
		SDL_Surface* bmpData = (SDL_Surface*)g_pScreenBuffer;

		SDL_UpdateTexture(
			g_pBackBuffer,
			nullptr,
			bmpData->pixels,
			bmpData->pitch
		);

		SDL_SetRenderTarget(g_pRenderer, nullptr);
		SDL_RenderClear(g_pRenderer);
		SDL_RenderTexture(g_pRenderer, g_pBackBuffer, nullptr, nullptr);
		SDL_RenderPresent(g_pRenderer);
	}
}