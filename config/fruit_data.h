#pragma once

#include "SDL.h"

#include <vector>

using namespace std;

struct FruitConfig {
	const char* name;
	const char* texture;
	int score;
	int maxHp;
	SDL_Color color;

};

class FruitData
{
public:
	static vector<FruitConfig> configs;

	static FruitConfig* GetFruitConfigByName(const char* name);

	static FruitConfig GetRandomFruitConfig() {
		int randomIndex = rand() % configs.size();
		return configs[randomIndex];
	}
};