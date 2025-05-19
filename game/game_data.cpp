#include "game_data.h"

//fn1v hash
inline const uint32_t HashGameDataString(std::string gameDataStr) {

	uint32_t hash = 0x811c9dc5;
	uint32_t prime = 0x1000193;

	for (int i = 0; i < gameDataStr.length(); ++i) {
		uint8_t value = gameDataStr[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

void GameData::ResetData() {
	this->highestScore = 0;
	this->highestComboAchieved = 0;
	this->longestTimeAlive = 0;
	this->bladeColor = BladeColorsConfig::GetBladeColorByName("default_blade");
}

std::string GameData::ToString() {
	return std::to_string(highestScore) + ":" + std::to_string(highestComboAchieved) + ":" +
		std::to_string(longestTimeAlive);
}

void GameData::ReloadBladeData() {
	for (auto bladeColor : BladeColorsConfig::colors) {
		bladeColor->isUnlocked = bladeColor->checkUnlock(this->highestScore, this->highestComboAchieved);
	}
}

GameData::GameData(std::string path) {
	this->path = path;
	std::fstream file(path, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		this->ResetData();
		this->Save();
		return;
	}

	uint64_t signature;

	file.read((char*)&this->timestamp, sizeof(uint64_t));
	file.read((char*)&signature, sizeof(uint64_t));

	uint32_t size;
	file.read((char*)&size, sizeof(uint32_t));
	uint8_t* buffer = new uint8_t[size];
	file.read((char*)buffer, size);

	//Encryption
	randomSeed.seed(this->timestamp);

	uint64_t key = randomSeed();

	for (int i = 0; i < size; i++) {
		uint8_t keyByte = ((uint8_t*)&key)[i % 8];
		buffer[i] ^= keyByte;
		key = randomSeed();
	}
	
	this->highestScore = *(int*)(buffer);
	this->highestComboAchieved = *(int*)(buffer + 4);
	this->longestTimeAlive = *(uint64_t*)(buffer + 8);

	this->ReloadBladeData();
	
	//ulgy lol, i should write a wrapper or something
	int bladeColorIdLength = *(int*)(buffer + 16);
	char* bladeColorId = new char[bladeColorIdLength + 1];
	memcpy(bladeColorId, buffer + 20, bladeColorIdLength);

	bladeColorId[bladeColorIdLength] = '\0';

	this->bladeColor = BladeColorsConfig::GetBladeColorByName(bladeColorId);

	if (this->bladeColor == nullptr) {
		SDL_Log("Blade color %s not found, using default blade color", bladeColorId);
		this->bladeColor = BladeColorsConfig::GetBladeColorByName("default_blade");
		return;
	}

	//Cleanup
	delete[] buffer;
	file.close();

	//Signature checking
	if (signature != HashGameDataString(this->ToString())) {
		this->ResetData();
		this->Save();
	}
}

void GameData::Save() {
	uint32_t size = 16 + 4 + strlen(this->bladeColor->id);
	uint8_t* buffer = new uint8_t[size];
	
	*(int*)(buffer) = this->highestScore;
	*(int*)(buffer + 4) = this->highestComboAchieved;
	*(uint64_t*)(buffer + 8) = this->longestTimeAlive;
	*(int*)(buffer + 16) = strlen(this->bladeColor->id);
	memcpy(buffer + 20, this->bladeColor->id, strlen(this->bladeColor->id));

	this->timestamp = time(0);

	this->randomSeed.seed(this->timestamp);

	uint64_t key = randomSeed();

	for (int i = 0; i < size; i++) {
		uint8_t keyByte = ((uint8_t*)&key)[i % 8];
		buffer[i] ^= keyByte;
		key = randomSeed();
	}

	uint64_t signature = HashGameDataString(this->ToString());

	std::fstream file(path, std::ios::out | std::ios::binary);

	if (!file.is_open()) {
		SDL_Log("Failed to open file for writing");
		return;
	}

	file.write((char*)&this->timestamp, sizeof(uint64_t));
	file.write((char*)&signature, sizeof(uint64_t));	
	file.write((char*)&size, sizeof(uint32_t));
	file.write((char*)buffer, size);
	delete[] buffer;
	file.close();
}