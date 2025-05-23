#pragma once

#include "SDL.h"
#include "SDL_mixer.h"

#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

class AudioManager
{
private:
	static AudioManager* instancePtr;
public:

	bool isPlayingMusic = false;

	Mix_Music* mainMenuMusic;
	std::unordered_map<const char*, Mix_Chunk*> audios;
	void Initialize();

	static AudioManager* GetInstance() {
		return instancePtr;
	}
	
	void PlayMusic();
	void HaltMusic();

	int PlaySound(const char* name, int loops = 0);

	bool LoadAudioByName(const char* name);
	Mix_Music* LoadMusicByName(const char* name);
	Mix_Chunk* GetAudioByName(const char* name);

	void ChangeMusicVolume(int volume);
	void ChangeSoundVolume(int volume);
};