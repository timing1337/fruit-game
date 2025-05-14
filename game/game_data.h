#pragma once

#include "SDL.h"
#include "config/blade_color.h"

#include <fstream>
#include <random>
#include <string>

using namespace std;

struct GameData
{
public:
	string path;

	uint64_t timestamp;

	int highestScore = 0;
	int highestComboAchieved = 0;
	uint64_t longestTimeAlive = 0;

	BladeColor* bladeColor;

	GameData(string path);
	void Save();
	void ResetData();
	void ReloadBladeData();
	string ToString();
private:
	mt19937_64 randomSeed;
};
