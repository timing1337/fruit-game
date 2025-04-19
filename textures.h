#pragma once

#include <unordered_map>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

struct Sprite {
	SDL_Texture* texture;
};

struct RenderedText {
	TTF_Font* font;

	int outline;

	SDL_Texture* text;
	SDL_Texture* outlineText;
};

enum GameTextureType {
	NONE = 0,
	SPRITE = 1,
	TEXT = 2
};

struct GameTexture {
	GameTextureType type;

	int width;
	int height;

	Sprite* sprite;
	RenderedText* text;
};