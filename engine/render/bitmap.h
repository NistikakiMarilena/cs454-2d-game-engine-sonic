#pragma once

#include <cstdint>

namespace gfx
{
	struct Point
	{
		int x = 0;
		int y = 0;
	};

	struct Rect
	{
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
	};

	typedef void* Bitmap;

	Bitmap LoadBitmap(const char* path);
	Bitmap CreateBitmap(int width, int height);
	int BitmapWidth(Bitmap bitmap);
	int BitmapHeight(Bitmap bitmap);
	void DeleteBitmap(Bitmap bmp);
	void ClearBitmap(Bitmap bmp, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void BlitBitmap(
		Bitmap srcBmp,
		Rect rect,
		Bitmap destBmp,
		Point point
	);
}
