#pragma once

#include "SDL.h"

#include <fstream>
#include <random>

using namespace std;

struct GameData
{
public:
	string path;

	uint64_t timestamp;
	uint64_t signature;

	uint32_t highestScore = 0;
	uint32_t highestComboAchived = 0;
	uint32_t longestTimeAlive = 0;

	GameData(string path);
	void Save();
private:
	mt19937 mt;
};
