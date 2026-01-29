#pragma once

#include "bitmap.h"

namespace gfx {
	void Init();
	void Shutdown();
	Bitmap GetScreenBuffer();
	void flush();
}