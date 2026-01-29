#include "bitmap.h"

#include "SDL3/SDL.h"
#include "stb_image.h"

namespace gfx
{
	Bitmap LoadBitmap(const char* path)
	{
		int w = 0, h = 0, ch = 0;
		uint8_t* data = stbi_load(path, &w, &h, &ch, 4);

		SDL_Surface* surf = SDL_CreateSurface(
			w, h, SDL_PIXELFORMAT_RGBA8888
		);

		SDL_LockSurface(surf);
		auto dst = (uint8_t*)surf->pixels;
		int dstPitch = surf->pitch;

		const auto src = (uint8_t*)data;
		int srcPitch = w * 4;

		for (int y = 0; y < h; ++y)
		{
			auto dstRow = (Uint32*)(dst + y * dstPitch);
			const uint8_t* srcRow = src + y * srcPitch;

			for (int x = 0; x < w; ++x)
			{
				const uint8_t* p = srcRow + x * 4;
				dstRow[x] = SDL_MapRGBA(
					SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888),
					nullptr,
					p[0], p[1], p[2], p[3]
				);
			}
		}

		SDL_UnlockSurface(surf);
		stbi_image_free(data);
		return (void*)surf;
	}

	Bitmap CreateBitmap(int width, int height)
	{
		// TODO: Make color API (follow savidis stuff or copy paste mine)
		SDL_Surface* bmp = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA8888);
		SDL_FillSurfaceRect(
			bmp,
			nullptr,
			SDL_MapRGBA(
				SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888),
				nullptr, 
				255, 255, 255, 255)
		);
		return (Bitmap)bmp;
	}

	void DeleteBitmap(Bitmap bmp)
	{
		SDL_Surface* surf = (SDL_Surface*)bmp;
		SDL_DestroySurface(surf);
	}

	void BlitBitmap(Bitmap srcBmp, Rect rect, Bitmap destBmp, Point point)
	{
		SDL_Surface* src = (SDL_Surface*)srcBmp;
		SDL_Surface* dest = (SDL_Surface*)destBmp;

		SDL_Rect srcRect = { rect.x, rect.y, rect.w, rect.h };
		SDL_Rect dstRect = { point.x, point.y, rect.w, rect.h };

		SDL_BlitSurface(
			src,
			&srcRect,
			dest,
			&dstRect
		);
	}
}
