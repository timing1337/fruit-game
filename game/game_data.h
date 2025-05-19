#pragma once

#include "SDL.h"
#include "config/blade_color.h"

#include <fstream>
#include <random>
#include <string>



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
