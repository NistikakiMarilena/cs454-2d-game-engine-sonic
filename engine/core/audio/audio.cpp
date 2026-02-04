#include "audio.h"

#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"

#include <iostream>

namespace core::audio 
{
	struct music_data
	{
		MIX_Audio* audio = nullptr;
		MIX_Track* track = nullptr;
	};

	static MIX_Mixer* gMixer = nullptr;

	void Init(const core::config::AudioConfig& config)
	{
		MIX_Init();

		SDL_AudioSpec mixer = {
			.format = SDL_AUDIO_F32,
			.channels = config.channels,
			.freq = config.freq,
		};

		gMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &mixer);
		MIX_SetMixerGain(gMixer, config.volume);
	}

	void Shutdown()
	{
		MIX_DestroyMixer(gMixer);
		MIX_Quit();
	}

	sound LoadSound(const char* path)
	{
		MIX_Audio* audio = MIX_LoadAudio(gMixer, path, false);
		std::cout << SDL_GetError();
		return (sound)(audio);
	}

	music LoadMusic(const char* path)
	{
		MIX_Audio* audio = MIX_LoadAudio(gMixer, path, false);
		MIX_Track* track = MIX_CreateTrack(gMixer);
		MIX_SetTrackAudio(track, audio);

		music_data* data = (music_data*)malloc(sizeof(music_data));
		data->track = track;
		data->audio = audio;

		return (music)data;
	}

	void  DestroySound(sound s)
	{
		MIX_Audio* data = (MIX_Audio*)(s);
		MIX_DestroyAudio(data);
	}

	void  DestroyMusic(music m)
	{
		music_data* data = (music_data*)(m);
		MIX_DestroyTrack(data->track);
		MIX_DestroyAudio(data->audio);
		free(data);
	}

	int PlaySound(sound s)
	{
		MIX_Audio* data = (MIX_Audio*)(s);
		return MIX_PlayAudio(gMixer, data);
	}

	int PlayMusic(music m, int loop)
	{
		music_data* data = (music_data*)(m);

		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loop);

		return MIX_PlayTrack(data->track, props);
	}

	int StopMusic(music m, int fade_out)
	{
		music_data* data = (music_data*)(m);
		Sint64 frames = MIX_AudioMSToFrames(data->audio, fade_out);
		return MIX_StopTrack(data->track, fade_out);
	}
}