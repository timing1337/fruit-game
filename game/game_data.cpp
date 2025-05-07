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

	/*
	* Simple XOR encryption for game-data to prevent player from tampering with scores
	* Using signature as a seed
	* For each byte it will generate an ulong and select a byte (depends on the position of the byte) and xor it with encrypted byte
	*/

	randomSeed.seed(this->signature);

	uint64_t key = randomSeed();

	for (int i = 0; i < size; i++) {
		uint8_t keyByte = ((uint8_t*)&key)[i % 8];
		buffer[i] ^= keyByte;
		key = randomSeed();
	}
	
	this->highestScore = *(int*)(buffer);
	this->highestComboAchived = *(int*)(buffer + 4);
	this->longestTimeAlive = *(uint64_t*)(buffer + 8);

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
	uint32_t size = 16;
	uint8_t* buffer = new uint8_t[size];
	
	*(int*)(buffer) = this->highestScore;
	*(int*)(buffer + 4) = this->highestComboAchived;
	*(uint64_t*)(buffer + 8) = this->longestTimeAlive;

	this->timestamp = time(0);
	this->signature = this->timestamp ^ this->highestScore | this->highestComboAchived ^ this->longestTimeAlive;

	this->randomSeed.seed(this->signature);

	uint64_t key = randomSeed();

	for (int i = 0; i < size; i++) {
		uint8_t keyByte = ((uint8_t*)&key)[i % 8];
		buffer[i] ^= keyByte;
		key = randomSeed();
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