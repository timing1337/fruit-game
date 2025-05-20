#pragma once

#include "SDL.h"
#include "config/blade_color.h"

#include <fstream>
#include <random>
#include <string>

//these are used as reference only
struct FilesaveStructure {
	uint64_t timestamp; // thời gian lưu file
	uint64_t signature; // signature kiểm tra data
	uint32_t dataSize;
	uint8_t* data; // game encrypted data
};
struct GameDataStructure {
	uint32_t highestScore;
	uint32_t highestCombo;
	uint64_t longestTimeAlive;
	std::string bladeColorId; //null-terminated string
};

struct GameData
{
public:
	std::string path;

	uint64_t timestamp;

	int highestScore = 0;
	int highestComboAchieved = 0;
	uint64_t longestTimeAlive = 0;

	BladeColor* bladeColor;

	GameData(std::string path);
	void Save();
	void ResetData();
	void ReloadBladeData();
	std::string ToString();
private:
	std::mt19937_64 randomSeed;
};
