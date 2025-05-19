#include "audio_mgr.h"

AudioManager* AudioManager::instancePtr = new AudioManager();

void AudioManager::Initialize() {

}

bool AudioManager::LoadAudioByName(const char* name) {
	return true;
}

