#pragma once

#include "SDL.h"

#include <functional>
#include <unordered_map>

struct BladeColor {
public:
	const char* id;
	const char* name;
	const char* description;
	const char* textureId;
	bool isUnlocked = false;
	SDL_Color color1;
	SDL_Color color2;

	//stupid but...i cant find any other way to make this better lol
	std::function<bool(int score, int combo)> checkUnlock;

	BladeColor(const char* id, const char* name, const char* description, const char* textureId, SDL_Color color1, SDL_Color color2, std::function<bool(int score, int combo)> checkUnlock) : id(id), name(name), description(description), textureId(textureId), color1(color1), color2(color2), checkUnlock(checkUnlock) { }
};

class BladeColorsConfig
{
public:
	static std::vector<BladeColor*> colors;

	static BladeColor* GetBladeColorByName(const char* name);
};

