#pragma once

#include "SDL.h"

#include <vector>

enum BuffId {
	BUFF_NONE,
	DOUBLE_SCORE, //combo trigger
	FREEZE, // freeze fruit
	FRUIT_PARTY //fruit_party
};

struct BuffConfig {
public:
	BuffId id;
	int duration;
	SDL_Color fruitColorChange;
	float chance;

	BuffConfig(BuffId id, int duration, SDL_Color fruitColorChange, float chance) : id(id), duration(duration), fruitColorChange(fruitColorChange), chance(chance){}
};

class BuffData
{
public:
	static std::vector<BuffConfig*> configs;

	static BuffConfig* GetRandomBuffConfig();

	static BuffConfig* GetBuffConfigById(BuffId id);
};