#include "audio_mgr.h"

AudioManager* AudioManager::instancePtr = new AudioManager();

void AudioManager::Initialize() {
	mainMenuMusic = LoadMusicByName("main_menu.wav");
	PlayMusic();
}

void AudioManager::PlayMusic() {
	if (isPlayingMusic) {
		return;
	}
	Mix_PlayMusic(mainMenuMusic, -1);
	isPlayingMusic = true;
}

void AudioManager::HaltMusic() {
	if (!isPlayingMusic) {
		return;
	}
	Mix_FadeOutMusic(1000);
	isPlayingMusic = false;
}

bool AudioManager::LoadAudioByName(const char* name) {
	Mix_Chunk* wav = Mix_LoadWAV((fs::current_path().string() + "/game_data/audio/" + name).c_str());
	if (wav == NULL) {
		SDL_Log(Mix_GetError());
		return false;
	}
	audios[name] = wav;
	return true;
}

Mix_Music* AudioManager::LoadMusicByName(const char* name) {
	Mix_Music* music = Mix_LoadMUS((fs::current_path().string() + "/game_data/audio/" + name).c_str());
	if (music == NULL) {
		SDL_Log(Mix_GetError());
		return nullptr;
	}
	return music;
}

Mix_Chunk* AudioManager::GetAudioByName(const char* name) {
	for (auto& audio : audios) {
		if (strcmp(audio.first, name) == 0) {
			return audio.second;
		}
	}
	if (!LoadAudioByName(name)) {
		return nullptr;
	}
	return audios[name];
}

//scale from 0-100
void AudioManager::ChangeMusicVolume(int volume) {
	int rescaled = (volume / 100.0f) * 128;
	SDL_Log("Volume: %d", rescaled);
	Mix_VolumeMusic(volume);
}

void AudioManager::ChangeSoundVolume(int volume) {
	int rescaled = (volume / 100.0f) * 128;
	SDL_Log("Volume: %d", rescaled);
	Mix_Volume(-1, volume);
}
