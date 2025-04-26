#include "game_data.h"

GameData::GameData(string path) {
	this->path = path;
	fstream file(path, ios::in | ios::binary);

	if (!file.is_open()) {
		this->Save();
		return;
	}

	file.read((char*)&this->timestamp, sizeof(uint64_t));
	file.read((char*)&this->signature, sizeof(uint64_t));

	uint32_t size;
	file.read((char*)&size, sizeof(uint32_t));
	uint8_t* buffer = new uint8_t[size];
	file.read((char*)buffer, size);

	this->mt.seed(this->signature);

	uint32_t key = mt();

	SDL_Log("Timestamp: %llu", this->timestamp);
	SDL_Log("Signature: %llu", this->signature);

	for (int i = 0; i < size; i++) {
		uint8_t keyByte = ((uint8_t*)&key)[i % 4];
		buffer[i] ^= keyByte;
		key = mt();
	}
	
	this->highestScore = *(uint32_t*)(buffer);
	this->highestComboAchived = *(uint32_t*)(buffer + 4);
	this->longestTimeAlive = *(uint32_t*)(buffer + 8);

	SDL_Log("Highest score: %d", this->highestScore);
	SDL_Log("Highest combo: %d", this->highestComboAchived);
	SDL_Log("Longest time alive: %d", this->longestTimeAlive);

	uint64_t signature = this->timestamp ^ this->highestScore | this->highestComboAchived ^ this->longestTimeAlive;

	if (signature != this->signature) {
		SDL_Log("Signature mismatch, resetting game data");
		this->highestScore = 0;
		this->highestComboAchived = 0;
		this->longestTimeAlive = 0;
	}

	delete[] buffer;
	file.close();
}

void GameData::Save() {
	SDL_Log("Saving game data");
	uint32_t size = 4 + 4 + 4;
	uint8_t* buffer = new uint8_t[size];
	
	*(uint32_t*)(buffer) = this->highestScore;
	*(uint32_t*)(buffer + 4) = this->highestComboAchived;
	*(uint32_t*)(buffer + 8) = this->longestTimeAlive;

	this->timestamp = time(0);
	this->signature = this->timestamp ^ this->highestScore | this->highestComboAchived ^ this->longestTimeAlive;

	this->mt.seed(this->signature);

	uint32_t key = mt();
	SDL_Log("Timestamp: %llu", this->timestamp);
	SDL_Log("Signature: %llu", this->signature);

	for (int i = 0; i < size; i++) {
		uint8_t keyByte = ((uint8_t*)&key)[i % 4];
		buffer[i] ^= keyByte;
		key = mt();
	}

	fstream file(path, ios::out | ios::binary);

	if (!file.is_open()) {
		SDL_Log("Failed to open file for writing");
		return;
	}

	file.write((char*)&this->timestamp, sizeof(uint64_t));
	file.write((char*)&this->signature, sizeof(uint64_t));	
	file.write((char*)&size, sizeof(uint32_t));
	file.write((char*)buffer, size);
	delete[] buffer;
	file.close();
}