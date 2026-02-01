#pragma once

#include "bitmap.h"
#include <string>

namespace gfx {
	void Init(const char* windowTitle,
		int windowWidth, int windowHeight,
		int sceneWidth, int sceneHeight);

	void Shutdown();

	Bitmap GetScreenBuffer();

	void flush();
}