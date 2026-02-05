#include "bitmap.h"

#include "SDL3/SDL.h"
#include "stb_image.h"

namespace gfx
{
	int BitmapWidth(Bitmap bitmap)
	{
		auto bmp = static_cast<SDL_Surface*>(bitmap);
		if (bmp)
			return bmp->w;
		else
			return -1;
	}

	int BitmapHeight(Bitmap bitmap)
	{
		auto bmp = static_cast<SDL_Surface*>(bitmap);
		if (bmp)
			return bmp->h;
		else
			return -1;
	}

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

	void ClearBitmap(Bitmap bmp, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		if (!bmp)
			return;

		SDL_Surface* surf = (SDL_Surface*)bmp;
		SDL_FillSurfaceRect(
			surf,
			nullptr,
			SDL_MapRGBA(
				SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888),
				nullptr,
				r, g, b, a
			)
		);
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

	void BlitBitmapFlippedX(Bitmap srcBmp, Rect rect, Bitmap destBmp, Point point)
	{
		SDL_Surface* src = (SDL_Surface*)srcBmp;
		SDL_Surface* dest = (SDL_Surface*)destBmp;
		if (!src || !dest)
			return;

		int rx = rect.x;
		int ry = rect.y;
		int rw = rect.w;
		int rh = rect.h;
		if (rw <= 0 || rh <= 0)
			return;

		if (rx < 0) { rw += rx; rx = 0; }
		if (ry < 0) { rh += ry; ry = 0; }
		if (rx + rw > src->w) rw = src->w - rx;
		if (ry + rh > src->h) rh = src->h - ry;
		if (rw <= 0 || rh <= 0)
			return;

		SDL_Surface* temp = SDL_CreateSurface(rw, rh, SDL_PIXELFORMAT_RGBA8888);
		if (!temp)
			return;

		SDL_LockSurface(src);
		SDL_LockSurface(temp);

		auto* srcPixels = (Uint8*)src->pixels;
		auto* tmpPixels = (Uint8*)temp->pixels;
		const int srcPitch = src->pitch;
		const int tmpPitch = temp->pitch;

		for (int y = 0; y < rh; ++y)
		{
			const Uint32* srcRow = (const Uint32*)(srcPixels + (ry + y) * srcPitch);
			Uint32* tmpRow = (Uint32*)(tmpPixels + y * tmpPitch);

			for (int x = 0; x < rw; ++x)
				tmpRow[x] = srcRow[rx + (rw - 1 - x)];
		}

		SDL_UnlockSurface(temp);
		SDL_UnlockSurface(src);

		SDL_Rect dstRect = { point.x, point.y, rw, rh };
		SDL_BlitSurface(temp, nullptr, dest, &dstRect);

		SDL_DestroySurface(temp);
	}
}
