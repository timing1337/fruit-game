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
	static vector<FruitConfig> configs;

public:
	static void Initialize();
	static void Release();

	static FruitConfig* GetFruitConfigByName(const char* name);
	static FruitConfig GetRandomFruitConfig();
};