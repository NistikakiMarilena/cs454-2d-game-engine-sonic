#pragma once

#include "core/config/config.h"

namespace core::audio
{
	typedef void* sound;
	typedef void* music;

	void Init(const core::config::AudioConfig& config = core::config::AudioConfig());
	void Shutdown();

	sound LoadSound(const char* path); 
	music LoadMusic(const char* path);
	void  DestroySound(sound s);
	void  DestroyMusic(music m);

	int PlaySound(sound s);
	int PlayMusic(music m, int loop);
	int StopMusic(music m, int fade_out);
}