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

	int highestScore = 0;
	int highestComboAchived = 0;
	uint64_t longestTimeAlive = 0;

	GameData(string path);
	void Save();
private:
	mt19937_64 randomSeed;
};
