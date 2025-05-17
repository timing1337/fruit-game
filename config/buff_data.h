#pragma once

#include "SDL.h"

#include <vector>

using namespace std;

enum BuffId {
	DOUBLE_SCORE, //combo trigger
	FREEZE, // freeze fruit
	FRUIT_PARTY //fruit_party
};

struct BuffConfig {
public:
	BuffId id;
	int duration;
	SDL_Color fruitColorChange;

	BuffConfig(BuffId id, int duration, SDL_Color fruitColorChange) : id(id), duration(duration), fruitColorChange(fruitColorChange){}
};

class BuffData
{
public:
	static vector<BuffConfig*> configs;

	static BuffConfig* GetRandomBuffConfig() {
		int randomIndex = rand() % configs.size();
		return configs[randomIndex];
	}
};