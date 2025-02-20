#pragma once

#include <unordered_map>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

namespace fruit_game {

	struct Sprite {
		SDL_Texture* texture;
		int width;
		int height;
	};

	struct RenderedText {
		TTF_Font* font;

		int outline;

		int width;
		int height;

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

		Sprite* sprite;
		RenderedText* text;
	};

	class TextureManager
	{
	public:
		std::unordered_map<std::string, GameTexture*> textures;

		TextureManager(SDL_Renderer* renderer);
		~TextureManager();
	};
}

