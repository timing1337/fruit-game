#include "game_data.h"

//fn1v hash
inline const uint32_t HashGameDataString(string gameDataStr) {

	uint32_t hash = 0x811c9dc5;
	uint32_t prime = 0x1000193;

	for (int i = 0; i < gameDataStr.length(); ++i) {
		uint8_t value = gameDataStr[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

GameData::GameData(string path) {
	this->path = path;
	fstream file(path, ios::in | ios::binary);

	if (!file.is_open()) {
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

	//Cleanup
	delete[] buffer;
	file.close();

	//Signature checking
	if (signature != HashGameDataString(this->ToString())) {
		SDL_Log("Signature mismatch, resetting game data, stop cheating bruh");
		this->highestScore = 0;
		this->highestComboAchieved = 0;
		this->longestTimeAlive = 0;
		this->Save();
	}
}

void GameData::Save() {
	uint32_t size = 16;
	uint8_t* buffer = new uint8_t[size];
	
	*(int*)(buffer) = this->highestScore;
	*(int*)(buffer + 4) = this->highestComboAchieved;
	*(uint64_t*)(buffer + 8) = this->longestTimeAlive;

	this->timestamp = time(0);

	this->randomSeed.seed(this->timestamp);

	uint64_t key = randomSeed();

	for (int i = 0; i < size; i++) {
		uint8_t keyByte = ((uint8_t*)&key)[i % 8];
		buffer[i] ^= keyByte;
		key = randomSeed();
	}

	uint64_t signature = HashGameDataString(this->ToString());

	fstream file(path, ios::out | ios::binary);

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