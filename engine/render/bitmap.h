#pragma once

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
	void DeleteBitmap(Bitmap bmp);
	void BlitBitmap(
		Bitmap srcBmp,
		Rect rect,
		Bitmap destBmp,
		Point point
	);
}