#pragma once

#include "SDL.h"
#include "SDL_mixer.h"

#include <unordered_map>

class AudioManager
{
private:
	static AudioManager* instancePtr;
public:
	std::unordered_map<const char*, Mix_Music*> audios;
	void Initialize();

	static AudioManager* GetInstance() {
		return instancePtr;
	}

	bool LoadAudioByName(const char* name);
};

